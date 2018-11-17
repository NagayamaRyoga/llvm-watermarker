#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

namespace
{
	const llvm::cl::opt<int> test_opt
	{
		"test_opt",
		llvm::cl::desc("Test pass option"),
		llvm::cl::value_desc("test"),
		llvm::cl::init(10),
	};

	/**
	 * @brief      First watermarker pass.
	 */
	class TestPass
		: public llvm::FunctionPass
	{
	public:
		static char ID; // Needed for LLVM.

		/**
		 * @brief      Constructor.
		 */
		explicit TestPass()
			: FunctionPass(ID)
		{
		}

		// Uncopyable, unmovable.
		TestPass(const TestPass&) =delete;
		TestPass(TestPass&&) =delete;

		TestPass& operator=(const TestPass&) =delete;
		TestPass& operator=(TestPass&&) =delete;

		~TestPass() =default;

		/**
		 * @brief      Initialization before pass is run.
		 *
		 * @param      module  Reference of the module.
		 *
		 * @return     ?
		 */
		bool doInitialization(llvm::Module& module) override
		{
			llvm::errs() << __FUNCTION__ << " : " << module.getName() << "\n";
			llvm::errs() << "test_opt: " << test_opt << "\n";

			return false;
		}

		/**
		 * @brief      Finalization after pass is run.
		 *
		 * @param      module  Reference of the module.
		 *
		 * @return     ?
		 */
		bool doFinalization(llvm::Module& module) override
		{
			llvm::errs() << __FUNCTION__ << " : " << module.getName() << "\n";

			return false;
		}

		/**
		 * @brief      Processes functions.
		 *
		 * @param      func  Reference to the function.
		 *
		 * @return     `true` if the function was changed.
		 */
		bool runOnFunction(llvm::Function& func) override
		{
			llvm::errs() << __FUNCTION__ << " : " << func.getName() << "\n";
			llvm::errs() << "    " << "bbs : " << func.getBasicBlockList().size() << "\n";

			if (func.getBasicBlockList().size() < 3)
			{
				return false;
			}

			// Swap 2nd and 3rd basic blocks.
			llvm::BasicBlock& entry_bb = func.getEntryBlock();
			llvm::BasicBlock* bb1 = entry_bb.getNextNode();
			llvm::BasicBlock* bb2 = bb1->getNextNode();

			bb1->moveAfter(bb2);

			return true;
		}
	};

	char TestPass::ID;

	// Register pass.
	const llvm::RegisterPass<TestPass> pass_registry =
	{
		"testpass",
		"Test pass",
	};
}
