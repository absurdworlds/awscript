#ifndef llvm_helpers_h
#define llvm_helpers_h

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>

namespace aw::script {

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
inline auto CreateEntryBlockAlloca(
	llvm::LLVMContext& context,
	llvm::Function* TheFunction,
	llvm::Type* type,
	llvm::StringRef VarName)
		-> llvm::AllocaInst*
{
	auto& entry_block = TheFunction->getEntryBlock();

	llvm::IRBuilder<> builder(&entry_block, entry_block.begin());
	return builder.CreateAlloca(type, nullptr, VarName);
}

bool IsATerminatorInst(llvm::Value* val) {
	if (auto* inst = dyn_cast<llvm::Instruction>(val))
		return inst->isTerminator();
	return false;
}

} // namespace aw::script

#endif // llvm_helpers_h
