#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include "../CircularBitStream.hpp"
#include "../PermutationTable.hpp"
#include "Opts.hpp"

namespace
{
	const llvm::cl::opt<nykk::pass::RangeOptValue<std::size_t>, false, nykk::pass::RangeOptParser<std::size_t, 2, 13>> fpartition_opt
	{
		"fpartition",
		llvm::cl::desc("Function partition number (2 ~ 10 default 10)"),
		llvm::cl::value_desc("size"),
		llvm::cl::init(nykk::pass::RangeOptValue<std::size_t> {10}),
	};

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

			auto functions = std::vector<std::reference_wrapper<llvm::Function>>();
			functions.reserve(module.size());

			for (auto& func : module.functions())
			{
				if (!func.empty())
				{
					functions.emplace_back(std::ref(func));
				}
			}

			if (functions.size() < 2)
			{
				llvm::errs() << module.getName() << ", " << functions.size() << ", " << num_embedded_bits << "\n";

				return false;
			}

			const auto partition = fpartition_opt.getValue().value;
			const auto perm_table = nykk::create_permutation_table(partition);

			// Table of {log2 x! | x < 13}
			constexpr auto possible_embedding_bits = std::array<std::size_t, 13>
			{
				0, 0, 1, 2, 4, 6, 9, 12, 15, 18, 21, 25, 28,
			};

			std::size_t index = 0;

			for (; index + partition <= functions.size(); index += partition)
			{
				// Part of watermark to embed.
				const auto data = bit_stream->read(possible_embedding_bits[partition]);

				// Shuffles each `partition` functions.
				for (std::size_t i = 0; i < partition; i++)
				{
					const auto n = index + perm_table.at(data).at(i);

					functions[n].get().removeFromParent();
					module.getFunctionList().push_back(&functions[n].get());
				}

				num_embedded_bits += possible_embedding_bits[partition];
			}

			// Inserts rest functions.
			for (; index < functions.size(); index++)
			{
				functions[index].get().removeFromParent();
				module.getFunctionList().push_back(&functions[index].get());
			}

			llvm::errs() << module.getName() << ", " << functions.size() << ", " << num_embedded_bits << "\n";

			return true;
		}
	};

	// Registers pass.
	const llvm::RegisterPass<FunctionWatermarkPass> pass_registry =
	{
		"func-wm",
		"Watermarking pass by function ordering methods",
	};
}
