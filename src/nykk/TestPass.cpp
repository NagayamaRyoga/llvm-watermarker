#include "TestPass.hpp"

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

// Register pass.
llvm::RegisterPass<nykk::TestPass> g_test_pass {
	"testpass",
	"Test pass",
};

namespace nykk
{
	// Pass id.
	char TestPass::ID;

	// Constructor of TestPass.
	TestPass::TestPass()
		: FunctionPass(ID)
	{
	}

	// Initialization before pass is run
	bool TestPass::doInitialization(llvm::Module& module)
	{
		llvm::errs() << __FUNCTION__ << " : " << module.getName() << "\n";

		return false;
	}

	// Finalization after pass is run
	bool TestPass::doFinalization(llvm::Module& module)
	{
		llvm::errs() << __FUNCTION__ << " : " << module.getName() << "\n";

		return false;
	}

	// Processes functions.
	bool TestPass::runOnFunction(llvm::Function& func)
	{
		llvm::errs() << __FUNCTION__ << " : " << func.getName() << "\n";

		return false;
	}
}
