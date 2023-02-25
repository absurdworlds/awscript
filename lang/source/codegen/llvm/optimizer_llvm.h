#ifndef optimizer_llvm_h
#define optimizer_llvm_h

#include <aw/script/codegen/backend.h>

#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Target/TargetMachine.h>

namespace aw::script {
class optimizer_llvm {

public:
	optimizer_llvm(optimization_level level, llvm::TargetMachine& machine);

	void run(llvm::Module& module);

private:
	llvm::LoopAnalysisManager     lam;
	llvm::FunctionAnalysisManager fam;
	llvm::CGSCCAnalysisManager    cgam;
	llvm::ModuleAnalysisManager   mam;

	llvm::ModulePassManager mpm;

};
} // namespace aw::script
#endif // optimizer_llvm_h
