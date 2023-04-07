#include "backend_llvm.h"

#include "llvm_helpers.h"
#include "optimizer_llvm.h"

#include <aw/script/diag/error_t.h>

#include <aw/utility/ranges/paired.h>
#include <aw/utility/ranges/ipairs.h>

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

	auto reloc_model = Reloc::Model::PIC_;

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

bool backend_llvm::create_module(const middle::module& mod)
{
	cur_module = std::make_unique<Module>(mod.name, context);
	cur_module->setDataLayout(target_machine->createDataLayout());

	for (const auto& decl : mod.decls)
	{
		handle_declaration(*decl);
	}

	gen_global_ctors();

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

auto backend_llvm::gen(const middle::declaration& decl) -> llvm::Value*
{
	return std::visit([this] (auto&& decl) { return gen(decl); }, decl);
}


auto get_llvm_type(llvm::LLVMContext& context, ir::type* type) -> llvm::Type*
{
	if (!type) {
		assert(!"Unresolved reference to type");
		return Type::getVoidTy(context);
	}

	auto name = type->name;
	if (auto ptr = get_if<ir::pointer_type>(&type->kind))
	{
		auto base_type = get_llvm_type(context, ptr->base_type);
		return PointerType::get(base_type, 0);
	}


	if (auto integer = get_if<ir::integer_type>(&type->kind))
	{
		return IntegerType::get(context, integer->size);
	}

	// TODO: create a map of types
	if (name == "void")
		return Type::getVoidTy(context);

	if (name == "bool")
		return Type::getInt1Ty(context);

	if (in(name, "f32", "float"))
		return Type::getFloatTy(context);

	if (in(name, "f64", "double"))
		return Type::getDoubleTy(context);

	assert(!"Unknown type");
	return Type::getVoidTy(context);
}

struct arg_info {
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

auto create_function(
	llvm::LLVMContext& context,
	llvm::Module* module,
	const middle::function& decl) -> llvm::Function*
{
	if (auto func = module->getFunction(decl.name))
		return func;

	std::vector<Type*> args_types(decl.args.size());
	for (auto&& [type, in_arg] : paired(args_types, decl.args))
	{
		type = get_llvm_type(context, in_arg->type);
	}

	Type* return_type = get_llvm_type(context, decl.return_type);

	auto signature = FunctionType::get(return_type, args_types, false);
	auto func = Function::Create(signature, Function::ExternalLinkage, decl.name, module);

	for (auto&& [llvm_arg, in_arg] : paired(func->args(), decl.args))
	{
		llvm_arg.setName(in_arg->name);
	}

	return func;
}

auto backend_llvm::gen(const middle::function& decl) -> llvm::Value*
{
	auto* func = create_function(context, cur_module.get(), decl);

	std::vector<arg_info> args;
	for (auto&& [arg, value] : paired(decl.args, func->args()))
	{
		arg_info info;
		info.value = &value;
		info.type = value.getType();
		info.name = value.getName();
		info.is_mutable = arg->access == ast::access::variable;
		args.push_back(info);
	}

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

auto backend_llvm::gen(const std::unique_ptr<middle::statement>& stmt) -> llvm::Value*
{
	return stmt ? gen(*stmt) : nullptr;
}


auto backend_llvm::gen(const middle::statement& stmt) -> llvm::Value*
{
	return std::visit([this] (auto&& stmt) { return gen(stmt); }, stmt);
}

auto backend_llvm::gen(const middle::decl_statement& stmt) -> llvm::Value*
{
	if (auto var = get_if<middle::variable>(stmt.decl.get()))
	{
		auto* alloca = builder.CreateAlloca(
			get_llvm_type(context, var->type), nullptr, var->name);

		if (var->value)
			builder.CreateStore(gen(var->value), alloca);

		symtab[var->name] = alloca;
	}

	return nullptr;
}


auto backend_llvm::gen_if_condition(const std::unique_ptr<middle::expression>& expr) -> llvm::Value*
{
	auto* cond_v = gen(expr);
	if (!cond_v)
		return nullptr;

	return cond_v;
}

auto backend_llvm::gen(const middle::if_else_statement& stmt) -> llvm::Value*
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
		function->insert(function->end(), else_bb);
		builder.SetInsertPoint(else_bb);
		auto else_v = gen(stmt.else_body);
		if (!else_v)
			return nullptr;
		if (!IsATerminatorInst(then_v))
			builder.CreateBr(merge_bb);
		else_bb = builder.GetInsertBlock();
	}

	function->insert(function->end(), merge_bb);
	builder.SetInsertPoint(merge_bb);

	return UndefValue::get(Type::getVoidTy(context));
}

auto backend_llvm::gen(const middle::statement_block& list) -> llvm::Value*
{
	Value* ret = UndefValue::get(Type::getVoidTy(context));
	for (const auto& stmt : list)
		ret = gen(stmt);
	return ret;
}

auto backend_llvm::gen(const middle::return_statement& stmt) -> llvm::Value*
{
	if (stmt.value)
		return builder.CreateRet(gen(*stmt.value));
	return builder.CreateRetVoid();
}

auto backend_llvm::gen(const middle::empty_statement& stmt) -> llvm::Value*
{
	return UndefValue::get(Type::getVoidTy(context));
}

auto backend_llvm::gen(const middle::numeric_literal& expr) -> llvm::Constant*
{
	auto type = get_llvm_type(context, expr.type);
	auto radix = unsigned(expr.base); // TODO: to_underlying
	if (auto integer = dyn_cast<IntegerType>(type))
		return ConstantInt::get(context, APInt(integer->getBitWidth(), expr.value, radix));
	if (type->isFloatTy())
		return ConstantFP::get(context, APFloat(APFloat::IEEEsingle(), expr.value));
	if (type->isDoubleTy())
		return ConstantFP::get(context, APFloat(APFloat::IEEEdouble(), expr.value));
	return nullptr;
}

auto backend_llvm::gen(const middle::value_expression& expr) -> llvm::Value*
{
	auto it = symtab.find(expr.name);
	if (it != symtab.end())
		return it->second;

	it = globals.find(expr.name);
	if (it != globals.end())
		return it->second;

	return error_undefined_variable(diag, expr.name);
}

auto backend_llvm::gen(const std::unique_ptr<middle::expression>& expr) -> llvm::Value*
{
	return expr ? gen(*expr) : nullptr;
}


auto backend_llvm::gen(const middle::expression& expr) -> llvm::Value*
{
	auto value = std::visit([this] (auto&& expr) -> llvm::Value* { return gen(expr); }, expr);
	if (!value)
		return value;

	auto* value_type = value->getType();
	auto* expr_type = get_llvm_type(context, expr.type);
	if (value_type->isPointerTy() && !expr_type->isPointerTy()) {
		if (auto global = dyn_cast<GlobalValue>(value))
			value = builder.CreateLoad(global->getValueType(), global, global->getName());
		if (auto alloca = dyn_cast<AllocaInst>(value))
			value = builder.CreateLoad(alloca->getAllocatedType(), alloca, value->getName());
	}
	return value;
}

// TODO: remove gen_lvalue and instead use the reference type
auto backend_llvm::gen_lvalue(const std::unique_ptr<middle::expression>& expr) -> llvm::Value*
{
	return expr ? gen_lvalue(*expr) : nullptr;
}


auto backend_llvm::gen_lvalue(const middle::expression& expr) -> llvm::Value*
{
	auto value = std::visit([this] (auto&& expr) -> llvm::Value* { return gen(expr); }, expr);
	if (!value)
		return value;

	auto* type = value->getType();
	return type->isPointerTy() ? value : nullptr;
}



bool requires_lvalue(const ir::binary_operator op)
{
	return op == ir::binary_operator::assignment;
}

auto backend_llvm::gen(const middle::binary_expression& expr) -> llvm::Value*
{
	auto* lhs = requires_lvalue(expr.op) ? gen_lvalue(expr.lhs) : gen(expr.lhs);
	auto* rhs = gen(expr.rhs);
	if (!lhs || !rhs)
		return nullptr;

	switch (expr.op) {
	case ir::binary_operator::minus:
		return builder.CreateSub(lhs, rhs, "subtmp");
	case ir::binary_operator::plus:
		return builder.CreateAdd(lhs, rhs, "addtmp");
	case ir::binary_operator::multiply:
		return builder.CreateMul(lhs, rhs, "multmp");
	case ir::binary_operator::less:
		return builder.CreateICmpSLT(lhs, rhs, "lttmp");
	case ir::binary_operator::less_unsigned:
		return builder.CreateICmpULT(lhs, rhs, "lttmp");
	case ir::binary_operator::less_fp:
		return builder.CreateFCmpOLT(lhs, rhs, "lttmp");
	case ir::binary_operator::greater:
		return builder.CreateICmpSGT(lhs, rhs, "gttmp");
	case ir::binary_operator::greater_unsigned:
		return builder.CreateICmpUGT(lhs, rhs, "gttmp");
	case ir::binary_operator::greater_fp:
		return builder.CreateFCmpOGT(lhs, rhs, "lttmp");
	case ir::binary_operator::divide:
		return builder.CreateSDiv(lhs, rhs, "divtmp");
	case ir::binary_operator::divide_unsigned:
		return builder.CreateUDiv(lhs, rhs, "divtmp");
	case ir::binary_operator::divide_fp:
		return builder.CreateFDiv(lhs, rhs, "divtmp");
	case ir::binary_operator::not_equal:
		return builder.CreateICmpNE(lhs, rhs, "netmp");
	case ir::binary_operator::equal:
		return builder.CreateICmpEQ(lhs, rhs, "netmp");
	case ir::binary_operator::assignment:
		return builder.CreateStore(rhs, lhs);
	}
	return nullptr;
}

auto backend_llvm::gen(const middle::unary_expression& expr) -> llvm::Value*
{
	auto* val = gen(expr.lhs);
	if (!val)
		return nullptr;
	switch (expr.op) {
	case ir::unary_operator::minus:
		return builder.CreateNeg(val, "neg");
	case ir::unary_operator::plus:
		return val;
	case ir::unary_operator::negation:
		return builder.CreateNot(val, "not");
	};

	return nullptr;
}


auto backend_llvm::gen(const middle::call_expression& expr) -> llvm::Value*
{
	Function* callee = cur_module->getFunction(expr.func_name);
	if (!callee) {
		if (!expr.func)
			return error_is_not_declared(diag, expr.func_name);

		callee = create_function(context, cur_module.get(), *expr.func);
	}

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

auto backend_llvm::gen(const middle::if_expression& expr) -> llvm::Value*
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

	function->insert(function->end(), else_bb);
	builder.SetInsertPoint(else_bb);
	auto* else_v = gen(expr.else_body);
	if (!else_v)
		return nullptr;
	builder.CreateBr(merge_bb);
	else_bb = builder.GetInsertBlock();

	function->insert(function->end(), merge_bb);
	builder.SetInsertPoint(merge_bb);

	auto* phi = builder.CreatePHI(then_v->getType(), 2, "ifetmp");

	phi->addIncoming(then_v, then_bb);
	phi->addIncoming(else_v, else_bb);

	return phi;
}

auto mangle_string(std::string_view str) -> std::string
{
	constexpr auto max_str_len = 8;
	std::string mstr;
	for (auto [i, c] : ipairs(str))
	{
		if (i > max_str_len)
			break;
		if (isAlnum(c))
			mstr += c;
	}
	return ".str_" + mstr;
}

auto backend_llvm::gen(const middle::string_literal& expr) -> llvm::Value*
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
			GlobalValue::PrivateLinkage, const_data, mangle_string(expr.value));
		string_global->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
		auto res = strings.emplace(expr.value, string_global);
		assert(res.second);
		it = res.first;
	}

	return it->second;
}


} // namespace aw::script
