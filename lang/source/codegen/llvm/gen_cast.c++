#include "backend_llvm.h"

namespace aw::script {
auto get_llvm_type(llvm::LLVMContext& context, ir::type* type) -> llvm::Type*;

struct cast_visitor {
	llvm::IRBuilder<>& builder;
	llvm::Value* value;
	llvm::Type*  type;

	auto create_cast(llvm::Instruction::CastOps op) -> llvm::Value*
	{
		return builder.CreateCast(op, value, type, "cast");
	}

	auto operator()(ir::integer_type to, ir::integer_type from) -> llvm::Value*
	{
		if (to.size > from.size) {
			return create_cast(to.is_signed || from.is_signed ?
				llvm::Instruction::SExt :
				llvm::Instruction::ZExt);
		}

		return create_cast(llvm::Instruction::Trunc);
	}

	auto operator()(ir::fp_type to, ir::integer_type from) -> llvm::Value*
	{
		return create_cast(from.is_signed ?
			llvm::Instruction::SIToFP :
			llvm::Instruction::UIToFP);
	}

	auto operator()(ir::integer_type to, ir::fp_type from) -> llvm::Value*
	{
		return create_cast(to.is_signed ?
			llvm::Instruction::FPToSI :
			llvm::Instruction::FPToUI);
	}

	auto operator()(ir::fp_type to, ir::fp_type from) -> llvm::Value*
	{
		return create_cast(to.size < from.size ?
			llvm::Instruction::FPTrunc :
			llvm::Instruction::FPExt);
	}

	auto operator()(ir::pointer_type to, ir::integer_type from) -> llvm::Value*
	{
		return create_cast(llvm::Instruction::IntToPtr);
	}

	auto operator()(ir::integer_type to, ir::pointer_type from) -> llvm::Value*
	{
		return create_cast(llvm::Instruction::PtrToInt);
	}

	template<typename From_type>
	auto create_bool_cast(From_type) -> llvm::Value*
	{
		assert("!Invalid cast");
		return create_cast(llvm::Instruction::BitCast);
	}

	auto create_bool_cast(ir::pointer_type) -> llvm::Value*
	{
		return builder.CreateICmpNE(
			value,
			llvm::Constant::getNullValue(value->getType())
		);
	}

	auto create_bool_cast(ir::fp_type) -> llvm::Value*
	{
		return builder.CreateFCmpONE(
			value,
			llvm::ConstantFP::get(value->getType(), 0)
		);
	}

	auto create_bool_cast(ir::integer_type) -> llvm::Value*
	{
		return builder.CreateICmpNE(
			value,
			llvm::ConstantInt::get(value->getType(), 0)
		);
	}


	template<typename From_type>
	auto operator()(ir::simple_type to, From_type from) -> llvm::Value*
	{
		if (to == ir::simple_type::boolean)
			return create_bool_cast(from);

		assert("!Invalid cast");
		return create_cast(llvm::Instruction::BitCast);
	}

	template<typename T, typename U>
	auto operator()(T to, U from) -> llvm::Value*
	{
		assert(!"Invalid cast");
		return create_cast(llvm::Instruction::BitCast);
	}
};

auto backend_llvm::gen(const middle::cast_expression& expr) -> llvm::Value*
{
	auto* lhs = gen(expr.lhs);
	if (!lhs)
		return nullptr;

	auto to_type = expr.to_type;
	auto from_type = expr.lhs->type;
	if (to_type == from_type)
		return lhs;

	cast_visitor visitor{
		.builder = builder,
		.value = lhs,
		.type = get_llvm_type(context, to_type),
	};

	return std::visit( visitor, to_type->kind, from_type->kind );
}

} // namespace aw::script
