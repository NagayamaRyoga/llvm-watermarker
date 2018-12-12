#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include "../CircularBitStream.hpp"
#include "Opts.hpp"

namespace
{
	/**
	 * @brief      Watermarking pass by instruction swapping methods.
	 */
	class InstructionWatermarkPass
		: public llvm::BasicBlockPass
	{
	public:
		static inline char ID; // Needed for LLVM.

		/**
		 * @brief      Constructor.
		 */
		explicit InstructionWatermarkPass()
			: BasicBlockPass(ID)
			, module_name_()
			, bit_stream_()
		{
		}

		// Uncopyable, unmovable.
		InstructionWatermarkPass(const InstructionWatermarkPass&) =delete;
		InstructionWatermarkPass(InstructionWatermarkPass&&) =delete;

		InstructionWatermarkPass& operator=(const InstructionWatermarkPass&) =delete;
		InstructionWatermarkPass& operator=(InstructionWatermarkPass&&) =delete;

		~InstructionWatermarkPass() =default;

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
			bit_stream_ = nykk::CircularBitStream::from_string(nykk::pass::watermark_opt);

			llvm::errs() << "func" << ", " << "insts" << ", " << "bits" << "\n";

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
			bit_stream_.reset();

			return false;
		}

		/**
		 * @brief      Processes basic blocks.
		 *
		 * @param      block  Reference to the basic block.
		 *
		 * @return     `true` if the function was changed.
		 */
		bool runOnBasicBlock(llvm::BasicBlock& block) override
		{
			llvm::errs() << block.getParent()->getName() << "," << block.size() << "," << 0 << "\n";
			return false;
		}

	private:
		std::string module_name_;
		std::unique_ptr<nykk::CircularBitStream> bit_stream_;
	};

	// Registers pass.
	const llvm::RegisterPass<InstructionWatermarkPass> pass_registry =
	{
		"inst-wm",
		"Watermarking pass by instruction swapping methods",
	};
}
