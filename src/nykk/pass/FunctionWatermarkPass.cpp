#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include "../CircularBitStream.hpp"
#include "../PermutationTable.hpp"
#include "Opts.hpp"

namespace
{
	/**
	 * @brief      Watermarking pass by function ordering methods.
	 */
	class FunctionWatermarkPass
		: public llvm::ModulePass
	{
	public:
		static inline char ID; // Needed for LLVM.

		/**
		 * @brief      Constructor.
		 */
		explicit FunctionWatermarkPass()
			: ModulePass(ID)
		{
		}

		// Uncopyable, unmovable.
		FunctionWatermarkPass(const FunctionWatermarkPass&) =delete;
		FunctionWatermarkPass(FunctionWatermarkPass&&) =delete;

		FunctionWatermarkPass& operator=(const FunctionWatermarkPass&) =delete;
		FunctionWatermarkPass& operator=(FunctionWatermarkPass&&) =delete;

		~FunctionWatermarkPass() =default;

		/**
		 * @brief      Processes modules.
		 *
		 * @param      module Reference to the module.
		 *
		 * @return     `true` if the module was changed.
		 */
		bool runOnModule(llvm::Module& module) override
		{
			const auto bit_stream = nykk::CircularBitStream::from_string(nykk::pass::watermark_opt);

			std::size_t num_embedded_bits = 0;
			bool is_changed = false;

			auto functions = std::vector<std::reference_wrapper<llvm::Function>>(module.getFunctionList().rbegin(), module.getFunctionList().rend());

			for (const auto& func : functions)
			{
				is_changed = true;
				func.get().removeFromParent();
				module.getFunctionList().push_back(&func.get());
			}

			llvm::errs() << module.getName() << ", " << module.size() << ", " << num_embedded_bits << "\n";

			return is_changed;
		}
	};

	// Registers pass.
	const llvm::RegisterPass<FunctionWatermarkPass> pass_registry =
	{
		"func-wm",
		"Watermarking pass by function ordering methods",
	};
}
