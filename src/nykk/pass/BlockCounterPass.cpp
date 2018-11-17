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
			, module_name_()
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
			module_name_ = module.getName();

			return false;
		}

		/**
		 * @brief      Finalization after pass is run.
		 *
		 * @param      module  Reference of the module.
		 *
		 * @return     ?
		 */
		bool doFinalization([[maybe_unused]] llvm::Module& module) override
		{
			module_name_.clear();

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
				<< "[BlockCounter - '" << func.getName() << "' in " << module_name_ << "] Basic blocks: " << func.size() << "\n";

			return false;
		}

	private:
		std::string module_name_;
	};

	char BlockCounterPass::ID;

	// Register pass.
	const llvm::RegisterPass<BlockCounterPass> pass_registry =
	{
		"count-block",
		"IR analyzer",
	};
}
