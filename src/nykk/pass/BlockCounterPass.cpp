#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

namespace
{
	/**
	 * @brief      Simplest IR analyzer.
	 */
	class BlockCounterPass
		: public llvm::FunctionPass
	{
	public:
		static char ID; // Needed for LLVM.

		/**
		 * @brief      Constructor.
		 */
		explicit BlockCounterPass()
			: FunctionPass(ID)
		{
		}

		// Uncopyable, unmovable.
		BlockCounterPass(const BlockCounterPass&) =delete;
		BlockCounterPass(BlockCounterPass&&) =delete;

		BlockCounterPass& operator=(const BlockCounterPass&) =delete;
		BlockCounterPass& operator=(BlockCounterPass&&) =delete;

		~BlockCounterPass() =default;

		/**
		 * @brief      Initialization before pass is run.
		 *
		 * @param      module  Reference of the module.
		 *
		 * @return     ?
		 */
		bool doInitialization(llvm::Module& module) override
		{
			llvm::errs()
				<< "[BlockCounter - module   ] start: " << module.getName() << "\n";

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
			llvm::errs()
				<< "[BlockCounter - module   ] finish: " << module.getName() << "\n";

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
			llvm::errs()
				<< "[BlockCounter - function ] '" << func.getName() << "' Basic blocks: " << func.getBasicBlockList().size() << "\n";

			return false;
		}
	};

	char BlockCounterPass::ID;

	// Register pass.
	llvm::RegisterPass<BlockCounterPass> pass_registry {
		"count-block",
		"IR analyzer",
	};
}
