#include "optimizer_llvm.h"

#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>

using namespace llvm;

namespace aw::script {


auto convert_optimization_level(optimization_level level) -> llvm::OptimizationLevel
{
	switch(level) {
	case optimization_level::O0:
		return OptimizationLevel::O0;
	case optimization_level::O1:
		return OptimizationLevel::O1;
	case optimization_level::O2:
		return OptimizationLevel::O2;
	case optimization_level::O3:
		return OptimizationLevel::O3;
	}

	return OptimizationLevel::O0;
}

aw::script::optimizer_llvm::optimizer_llvm(optimization_level level, llvm::TargetMachine& machine)
{
	PassBuilder pb(&machine);

	pb.registerModuleAnalyses(mam);
	pb.registerCGSCCAnalyses(cgam);
	pb.registerFunctionAnalyses(fam);
	pb.registerLoopAnalyses(lam);
	pb.crossRegisterProxies(lam, fam, cgam, mam);

	mpm = pb.buildPerModuleDefaultPipeline(convert_optimization_level(level));
}

void optimizer_llvm::run(llvm::Module& module)
{
	mpm.run(module, mam);
}

} // namespace aw::script
