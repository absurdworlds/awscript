#include "backend_llvm.h"

#include <aw/types/array_view.h>

namespace aw::script {

auto get_llvm_type(llvm::LLVMContext& context, ir::type* type) -> llvm::Type*;

auto create_global_ctor_sig(llvm::LLVMContext& context) -> llvm::FunctionType*
{
	return llvm::FunctionType::get(llvm::Type::getVoidTy(context), {}, false);
}

auto create_global_ctor_decl(llvm::Module& module, std::string_view var_name) -> llvm::Function*
{
	auto signature = create_global_ctor_sig(module.getContext());

	llvm::Twine ctor_name(".init_", var_name);

	return llvm::Function::Create(signature, llvm::Function::InternalLinkage, ctor_name, &module);
}

void backend_llvm::create_global_ctor_body(llvm::Function* func, llvm::GlobalVariable* var, const middle::expression& expr)
{
	auto entry = llvm::BasicBlock::Create(func->getContext(), "entry", func);
	builder.SetInsertPoint(entry);
	auto init = gen(expr); // TODO: create a separate visitor class that takes a reference to IRBuilder
	if (!init) {
		assert(!"Codegen didn't work");
		return;
	}
	builder.CreateStore(init, var);
	builder.CreateRetVoid();
}

//! Create a global constructor declaration for the given variable.
auto backend_llvm::create_global_ctor(llvm::GlobalVariable* var, const middle::variable& decl) -> llvm::Function*
{
	auto* func = create_global_ctor_decl(*cur_module, decl.name);

	create_global_ctor_body(func, var, *decl.value);

	return func;
}

auto backend_llvm::gen(const middle::variable& decl) -> llvm::Value*
{
	auto* type = get_llvm_type(context, decl.type);

	auto null_initializer = llvm::Constant::getNullValue(type);

	auto var = new llvm::GlobalVariable(
		*cur_module, type,
		decl.access == ast::access::constant,
		llvm::GlobalValue::ExternalLinkage, nullptr, decl.name);

	if (decl.value) {
		if (auto num = get_if<middle::numeric_literal>(decl.value.get())) {
			var->setInitializer(gen(*num));
		} else {
			var->setInitializer(null_initializer);
			auto* func = create_global_ctor(var, decl);
			global_ctors.push_back({ 65535, func });
		}

	} else {
		var->setInitializer(null_initializer);
	}

	globals[decl.name] = var;

	return var;
}

auto backend_llvm::gen(const middle::struct_decl& decl) -> llvm::Value*
{
	std::vector<llvm::Type*> members;
	for (const auto& field : decl.members)
	{
		members.push_back(get_llvm_type(context, field->type));
	}

	llvm::StructType::create(context, members, decl.name);

	return nullptr;
}


template<typename Elem>
auto to_llvm(array_ref<Elem> ref) -> llvm::ArrayRef<Elem>
{
	return { ref.data(), ref.size() };
}

auto create_or_get_global_array(
	llvm::Module& module,
	std::string_view var_name,
	llvm::Type* elem_type,
	array_ref<llvm::Constant*> init) -> llvm::GlobalVariable*
{
	auto* var = module.getGlobalVariable(var_name);
	if (var)
		return var;

	auto* array_type = llvm::ArrayType::get(elem_type, init.size());
	auto* init_array = llvm::ConstantArray::get(array_type, to_llvm(init));

	return new llvm::GlobalVariable(
		module, array_type, true,
		llvm::GlobalValue::AppendingLinkage,
		init_array, var_name);
}

//! Generate the global ctor table
void backend_llvm::gen_global_ctors()
{
	if (global_ctors.empty())
		return;

	using namespace std::string_view_literals;

	auto pointer_type = llvm::PointerType::getUnqual(context);

	auto* ctor_elem_type = llvm::StructType::get(
		builder.getInt32Ty(), // Priority
		pointer_type, // Function pointer
		pointer_type // Data pointer
	);

	std::vector<llvm::Constant*> init;

	for (const auto& [priority, func] : global_ctors) {
		auto* ctor_elem = llvm::ConstantStruct::get(
			ctor_elem_type,
			builder.getInt32(priority),
			func,
			llvm::ConstantPointerNull::get(pointer_type)
		);
		init.push_back(ctor_elem);
	}

	create_or_get_global_array(*cur_module, "llvm.global_ctors", ctor_elem_type, init);
}

} // namespace aw::script
