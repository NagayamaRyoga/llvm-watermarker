#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

namespace
{
	/**
	 * @brief      Simplest IR analyzer.
	 */
	class CounterPass
		: public llvm::FunctionPass
	{
	public:
		static char ID; // Needed for LLVM.

		/**
		 * @brief      Constructor.
		 */
		explicit CounterPass()
			: FunctionPass(ID)
		{
		}

		// Uncopyable, unmovable.
		CounterPass(const CounterPass&) =delete;
		CounterPass(CounterPass&&) =delete;

		CounterPass& operator=(const CounterPass&) =delete;
		CounterPass& operator=(CounterPass&&) =delete;

		~CounterPass() =default;

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
				<< "[Counter - module   ] start: " << module.getName() << "\n";

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
				<< "[Counter - module   ] finish: " << module.getName() << "\n";

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
				<< "[Counter - function ] '" << func.getName() << "' Basic blocks: " << func.getBasicBlockList().size() << "\n";

			return false;
		}
	};

	char CounterPass::ID;

	// Register pass.
	llvm::RegisterPass<CounterPass> counter_pass_registry {
		"counterpass",
		"IR analyzer",
	};
}
