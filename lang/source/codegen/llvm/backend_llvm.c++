#include "backend_llvm.h"

#include "llvm_helpers.h"
#include "optimizer_llvm.h"

#include <aw/script/diag/error_t.h>
#include <aw/script/symtab/scope.h>

#include <llvm/IR/Constant.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <iostream>

using namespace llvm;

namespace aw::script {

static error_t error_undefined_variable(diagnostics_engine& diag, string_view name)
{
	return error(diag, diagnostic_id::undefined_variable, location(), name);
}

static error_t error_not_implemented_yet(diagnostics_engine& diag)
{
	return error(diag, diagnostic_id::not_implemented_yet, location());
}

static error_t error_is_not_declared(diagnostics_engine& diag, string_view name)
{
	return error(diag, diagnostic_id::is_not_declared, location(), name);
}


backend_llvm::backend_llvm(diagnostics_engine& diag)
	: diag(diag)
{
#if AWSCRIPT_DEBUG_LLVM
	// I don't want to pull everything, because it takes forever to load
	// the debugger, so I link only what I need
	LLVMInitializeX86TargetInfo();
	LLVMInitializeX86Target();
	LLVMInitializeX86TargetMC();
	LLVMInitializeX86AsmParser();
	LLVMInitializeX86AsmPrinter();
#else
	InitializeAllTargetInfos();
	InitializeAllTargets();
	InitializeAllTargetMCs();
	InitializeAllAsmParsers();
	InitializeAllAsmPrinters();
#endif
}

backend_llvm::~backend_llvm()
{

}

bool backend_llvm::set_target(string_view request_triple)
{
	using namespace std::string_view_literals;

	const std::string target_triple = request_triple.empty() ?
		sys::getDefaultTargetTriple() :
		std::string(request_triple);

	std::string error;
	auto target = TargetRegistry::lookupTarget(target_triple, error);
	if (!target) {
		errs() << error;
		return false;
	}

	auto cpu = "generic"sv;
	auto features = string_view{};

	TargetOptions opt;

	auto reloc_model = Optional<Reloc::Model>();

	target_machine = target->createTargetMachine(target_triple, cpu, features, opt, reloc_model);
	return true;
}

void backend_llvm::set_optimization_level(optimization_level level)
{
	if (level == optimization_level::O0) {
		optimizer.reset();
	} else {
		assert(target_machine);
		optimizer = std::make_unique<optimizer_llvm>(level, *target_machine);
	}
}

bool backend_llvm::create_module(string_view name)
{
	cur_module = std::make_unique<Module>(name, context);
	cur_module->setDataLayout(target_machine->createDataLayout());
	return true;
}

bool backend_llvm::optimize_module()
{
	if (!optimizer)
		return false;
	optimizer->run(*cur_module);
	return true;
}

bool backend_llvm::optimize_lto()
{
	return true;
}

bool backend_llvm::write_object_file(string_view out_path)
{
	std::error_code EC;
	raw_fd_ostream dest(out_path, EC, sys::fs::OF_None);

	if (EC) {
		errs() << "Could not open file: " << EC.message();
		return false;
	}

	legacy::PassManager emit_pass;
	auto FileType = CGFT_ObjectFile;

	if (target_machine->addPassesToEmitFile(emit_pass, dest, nullptr, FileType)) {
		errs() << "TheTargetMachine can't emit a file of this type";
		return false;
	}

	emit_pass.run(*cur_module);

	dest.flush();

	return true;
}

void backend_llvm::dump_ir()
{
	cur_module->print(errs(), nullptr);
}

auto backend_llvm::gen(const ast::declaration& decl) -> llvm::Value*
{
	return std::visit([this] (auto&& decl) { return gen(decl); }, decl);
}

struct backend_llvm::arg_info {
	std::string_view name;
	llvm::Type* type = nullptr;
	llvm::Argument* value = nullptr;
	bool is_mutable = false;

	void set_value(llvm::Argument& arg)
	{
		value = &arg;
		arg.setName(name);
	}
};


auto get_llvm_type(llvm::LLVMContext& context, std::string_view type) -> llvm::Type*
{
	if (type.empty()) {
		assert(!"Unresolved reference to type");
		return Type::getVoidTy(context);
	}

	// TODO: create a map of types
	if (type == "void")
		return Type::getVoidTy(context);

	if (type == "cstring")
		return PointerType::getInt8PtrTy(context);

	if (type == "bool")
		return Type::getInt1Ty(context);

	if (in(type, "i32", "int32", "int"))
		return Type::getInt32Ty(context);

	if (in(type, "i64", "int64"))
		return Type::getInt64Ty(context);

	if (in(type, "f32", "float32", "float"))
		return Type::getFloatTy(context);

	if (in(type, "f64", "float64", "double"))
		return Type::getDoubleTy(context);

	assert(!"Unknown type");
	return Type::getVoidTy(context);
}

auto backend_llvm::create_function(const ast::function& decl, const std::vector<arg_info>& args)
	-> llvm::Function*
{
	std::vector<Type*> args_types(args.size());
	std::transform(args.begin(), args.end(), args_types.begin(),
	               [] (const arg_info& arg) { return arg.type; });

	Type* return_type = get_llvm_type(context, decl.return_type);

	auto signature = FunctionType::get(return_type, args_types, false);
	auto func = Function::Create(signature, Function::ExternalLinkage, decl.name, cur_module.get());
	return func;
}

auto backend_llvm::gen(const ast::function& decl) -> llvm::Value*
{
	std::vector<arg_info> args;

	for (const auto& arg : decl.args)
	{
		arg_info info;
		info.type = get_llvm_type(context, arg.type);
		info.name = arg.name;
		info.is_mutable = arg.access == ast::access::variable;
		args.push_back(info);
	}

	auto* func = create_function(decl, args);

	unsigned i = 0;
	for (auto& arg : func->args())
		args[i++].set_value(arg);

	// TODO: this is temporary, until we have FFI modules
	if (!decl.body)
		return func;

	auto* bb = BasicBlock::Create(context, "entry", func);
	builder.SetInsertPoint(bb);

	symtab.clear();
	for (auto& arg : args)
	{
		if (!arg.is_mutable) {
			symtab[std::string(arg.name)] = arg.value;
		} else {
			auto* alloca = CreateEntryBlockAlloca(context, func, arg.type, arg.name);

			builder.CreateStore(arg.value, alloca);

			symtab[std::string(arg.name)] = alloca;
		}
	}

	if (Value* res = gen(decl.body)) {
		if (!llvm::isa<ReturnInst>(res)) {
			if (func->getReturnType()->isVoidTy())
				builder.CreateRetVoid();
			else
				builder.CreateRet(res);
		}

		llvm::verifyFunction(*func);

		return func;
	}

	// TODO: this bit crashes sometimes
	func->eraseFromParent();
	return nullptr;
}

auto backend_llvm::gen(const ast::variable& decl) -> llvm::Value*
{
	return nullptr;
}

auto backend_llvm::gen(const std::unique_ptr<ast::statement>& stmt) -> llvm::Value*
{
	return stmt ? gen(*stmt) : nullptr;
}


auto backend_llvm::gen(const ast::statement& stmt) -> llvm::Value*
{
	return std::visit([this] (auto&& stmt) { return gen(stmt); }, stmt);
}


auto backend_llvm::gen_if_condition(const std::unique_ptr<ast::expression>& expr) -> llvm::Value*
{
	auto* cond_v = gen(expr);
	if (!cond_v)
		return nullptr;

	return cond_v;
}

auto backend_llvm::gen(const ast::if_else_statement& stmt) -> llvm::Value*
{
	auto* cond_v = gen_if_condition(stmt.if_expr);
	if (!cond_v)
		return nullptr;

	auto* function = builder.GetInsertBlock()->getParent();

	auto* then_bb  = BasicBlock::Create(context, "then", function);
	auto* merge_bb = BasicBlock::Create(context, "ifcont");
	auto* else_bb  = !stmt.else_body ? merge_bb : BasicBlock::Create(context, "else");

	builder.CreateCondBr(cond_v, then_bb, else_bb);

	builder.SetInsertPoint(then_bb);

	auto then_v = gen(stmt.if_body);
	if (!then_v)
		return nullptr;
	if (!IsATerminatorInst(then_v))
		builder.CreateBr(merge_bb);
	// Codegen can change the current block, update then_bb for the PHI.
	then_bb = builder.GetInsertBlock();

	if (stmt.else_body) {
		function->getBasicBlockList().push_back(else_bb);
		builder.SetInsertPoint(else_bb);
		auto else_v = gen(stmt.else_body);
		if (!else_v)
			return nullptr;
		if (!IsATerminatorInst(then_v))
			builder.CreateBr(merge_bb);
		else_bb = builder.GetInsertBlock();
	}

	function->getBasicBlockList().push_back(merge_bb);
	builder.SetInsertPoint(merge_bb);

	return UndefValue::get(Type::getVoidTy(context));
}

auto backend_llvm::gen(const ast::statement_block& list) -> llvm::Value*
{
	Value* ret = UndefValue::get(Type::getVoidTy(context));
	for (const auto& stmt : list)
		ret = gen(stmt);
	return ret;
}

auto backend_llvm::gen(const ast::return_statement& stmt) -> llvm::Value*
{
	if (stmt.value)
		return builder.CreateRet(gen(*stmt.value));
	return builder.CreateRetVoid();
}

auto backend_llvm::gen(const ast::empty_statement& stmt) -> llvm::Value*
{
	return UndefValue::get(Type::getVoidTy(context));
}

auto backend_llvm::gen(const ast::numeric_literal& expr) -> llvm::Value*
{
	auto type = get_llvm_type(context, "int"); // TODO:
	auto radix = unsigned(expr.base); // TODO: to_underlying
	if (auto integer = dyn_cast<IntegerType>(type))
		return ConstantInt::get(context, APInt(integer->getBitWidth(), expr.value, radix));
	return nullptr;
}

auto backend_llvm::gen(const ast::value_expression& expr) -> llvm::Value*
{
	auto it = symtab.find(expr.name);
	if (it == symtab.end())
		return error_undefined_variable(diag, expr.name);
	return it->second;
}

auto backend_llvm::gen(const std::unique_ptr<ast::expression>& expr) -> llvm::Value*
{
	return expr ? gen(*expr) : nullptr;
}


auto backend_llvm::gen(const ast::expression& expr) -> llvm::Value*
{
	auto value = std::visit([this] (auto&& expr) { return gen(expr); }, expr);
	if (!value)
		return value;

	auto* type = value->getType();
	if (type->isPointerTy()) {
		if (auto alloca = dyn_cast<AllocaInst>(value))
			value = builder.CreateLoad(alloca->getAllocatedType(), alloca, value->getName());
	}
	return value;
}

auto backend_llvm::gen_lvalue(const std::unique_ptr<ast::expression>& expr) -> llvm::Value*
{
	return expr ? gen_lvalue(*expr) : nullptr;
}


auto backend_llvm::gen_lvalue(const ast::expression& expr) -> llvm::Value*
{
	auto value = std::visit([this] (auto&& expr) { return gen(expr); }, expr);
	if (!value)
		return value;

	auto* type = value->getType();
	return type->isPointerTy() ? value : nullptr;
}



bool requires_lvalue(const ast::binary_operator op)
{
	return op == ast::binary_operator::assignment;
}

auto backend_llvm::gen(const ast::binary_expression& expr) -> llvm::Value*
{
	auto* lhs = requires_lvalue(expr.op) ? gen_lvalue(expr.lhs) : gen(expr.lhs);
	auto* rhs = gen(expr.rhs);
	if (!lhs || !rhs)
		return nullptr;

	switch (expr.op) {
	case ast::binary_operator::minus:
		return builder.CreateSub(lhs, rhs, "subtmp");
	case ast::binary_operator::plus:
		return builder.CreateAdd(lhs, rhs, "addtmp");
	case ast::binary_operator::multiply:
		return builder.CreateMul(lhs, rhs, "multmp");
	case ast::binary_operator::less:
		return builder.CreateICmpSLT(lhs, rhs, "lttmp");
	case ast::binary_operator::greater:
		return builder.CreateICmpSGT(lhs, rhs, "gttmp");
	case ast::binary_operator::divide:
		return builder.CreateSDiv(lhs, rhs, "divtmp");
	//case ast::binary_operator::not_equal:
	//builder.CreateICmpNE(lhs, rhs, "netmp");
	case ast::binary_operator::assignment:
		return builder.CreateStore(rhs, lhs);
	}
	return nullptr;
}

auto backend_llvm::gen(const ast::unary_expression& expr) -> llvm::Value*
{
	auto* val = gen(expr.lhs);
	if (!val)
		return nullptr;
	switch (expr.op) {
	case ast::unary_operator::minus:
		return builder.CreateNeg(val, "neg");
	case ast::unary_operator::plus:
		return val;
	case ast::unary_operator::logical_negation:
		// should work only on bools
		[[fallthrough]];
	case ast::unary_operator::binary_negation:
		return builder.CreateNot(val, "not");
	};

	return nullptr;
}


auto backend_llvm::gen(const ast::call_expression& expr) -> llvm::Value*
{
	Function* callee = cur_module->getFunction(expr.func);
	if (!callee)
		return error_is_not_declared(diag, expr.func);

	if (callee->arg_size() != expr.args.size())
		return error_not_implemented_yet(diag); // argument mismatch

	std::vector<Value *> argv;
	for (const auto& arg : expr.args)
	{
		auto* res = gen(arg);
		if (!res)
			return nullptr;
		argv.push_back(res);
	}

	return callee->getReturnType()->isVoidTy() ?
		builder.CreateCall(callee, argv):
		builder.CreateCall(callee, argv, "calltmp");
}

auto backend_llvm::gen(const ast::if_expression& expr) -> llvm::Value*
{
	auto* cond_v = gen_if_condition(expr.if_expr);
	if (!cond_v)
		return nullptr;

	auto* function = builder.GetInsertBlock()->getParent();

	auto* then_bb  = BasicBlock::Create(context, "ifethen", function);
	auto* else_bb  = BasicBlock::Create(context, "ifeelse");
	auto* merge_bb = BasicBlock::Create(context, "ifecont");

	builder.CreateCondBr(cond_v, then_bb, else_bb);

	builder.SetInsertPoint(then_bb);
	auto* then_v = gen(*expr.if_body);
	if (!then_v)
		return nullptr;
	builder.CreateBr(merge_bb);
	then_bb = builder.GetInsertBlock();

	function->getBasicBlockList().push_back(else_bb);
	builder.SetInsertPoint(else_bb);
	auto* else_v = gen(expr.else_body);
	if (!else_v)
		return nullptr;
	builder.CreateBr(merge_bb);
	else_bb = builder.GetInsertBlock();

	function->getBasicBlockList().push_back(merge_bb);
	builder.SetInsertPoint(merge_bb);

	auto* phi = builder.CreatePHI(then_v->getType(), 2, "ifetmp");

	phi->addIncoming(then_v, then_bb);
	phi->addIncoming(else_v, else_bb);

	return phi;
}

auto mangle_string(std::string_view str) -> std::string
{
	// Just for fun. In the future I'll replace it
	// TODO: replace with a proper mangling scheme
	auto mstr = std::string(str.substr(0, 8));
	int i = 0;
	for (auto& c : mstr)
	{
		if (!isAlnum(c))
			c = '0' + char(i++ % 10);
	}
	return "_str_" + mstr;
}

auto backend_llvm::gen(const ast::string_literal& expr) -> llvm::Value*
{
	using namespace std::string_literals;
	auto it = strings.find(expr.value);
	if (it == strings.end())
	{
		// TODO: strings in awscript are not zero-terminated
		// but I don't have a proper string type for now
		bool is_zstring = true;
		auto* const_data = ConstantDataArray::getString(context, expr.value, is_zstring);
		auto* string_global = new GlobalVariable(
			*cur_module, const_data->getType(), true,
			GlobalValue::ExternalLinkage, const_data, mangle_string(expr.value));
		auto res = strings.emplace(expr.value, string_global);
		assert(res.second);
		it = res.first;
	}

	return it->second;
}


} // namespace aw::script
