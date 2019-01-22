#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include "../CircularBitStream.hpp"
#include "../PermutationTable.hpp"
#include "Opts.hpp"

namespace
{
	const llvm::cl::opt<nykk::pass::RangeOptValue<std::size_t>, false, nykk::pass::RangeOptParser<std::size_t, 2, 10>> partition_opt
	{
		"partition",
		llvm::cl::desc("Block partition number (2 ~ 10 default 7)"),
		llvm::cl::value_desc("size"),
		llvm::cl::init(nykk::pass::RangeOptValue<std::size_t> {7}),
	};

	/**
	 * @brief      Watermarking pass by code layout methods.
	 */
	class BlockWatermarkPass
		: public llvm::FunctionPass
	{
	public:
		static inline char ID; // Needed for LLVM.

		/**
		 * @brief      Constructor.
		 */
		explicit BlockWatermarkPass()
			: FunctionPass(ID)
			, module_name_()
			, bit_stream_()
			, perm_table_()
		{
		}

		// Uncopyable, unmovable.
		BlockWatermarkPass(const BlockWatermarkPass&) =delete;
		BlockWatermarkPass(BlockWatermarkPass&&) =delete;

		BlockWatermarkPass& operator=(const BlockWatermarkPass&) =delete;
		BlockWatermarkPass& operator=(BlockWatermarkPass&&) =delete;

		~BlockWatermarkPass() =default;

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
			perm_table_ = nykk::create_permutation_table(partition_opt.getValue().value);

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
		 * @brief      Processes functions.
		 *
		 * @param      func  Reference to the function.
		 *
		 * @return     `true` if the function was changed.
		 */
		bool runOnFunction(llvm::Function& func) override
		{
			llvm::errs() << func.getName() << ", " << func.size() << ", ";

			const auto partition = partition_opt.getValue().value;

			if (func.size() <= partition)
			{
				llvm::errs() << 0 << "\n";

				return false;
			}

			// Table of {log2 x! | x < 13}
			constexpr auto possible_embedding_bits = std::array<std::size_t, 13>
			{
				0, 0, 1, 2, 4, 6, 9, 12, 15, 18, 21, 25, 28,
			};

			const auto blocks = std::vector<std::reference_wrapper<llvm::BasicBlock>>(std::begin(func), std::end(func));
			auto last_block = &func.getEntryBlock();

			// Embeds watermark.
			std::size_t num_embedded_bits = 0;
			std::size_t block_index = 1; // Without entry block.

			for (; block_index + partition <= blocks.size(); block_index += partition)
			{
				// Part of watermark to embed.
				const auto data = bit_stream_->read(possible_embedding_bits[partition]);

				// Shuffles each `partition` blocks.
				for (std::size_t i = 0; i < partition; i++)
				{
					const auto index = block_index + perm_table_.at(data).at(i);

					blocks[index].get().moveAfter(last_block);
					last_block = &blocks[index].get();
				}

				num_embedded_bits += possible_embedding_bits[partition];
			}

			// Inserts rest blocks.
			for (; block_index < blocks.size(); block_index++)
			{
				blocks[block_index].get().moveAfter(last_block);
				last_block = &blocks[block_index].get();
			}

			llvm::errs() << num_embedded_bits << "\n";

			return true;
		}

	private:
		std::string module_name_;
		std::unique_ptr<nykk::CircularBitStream> bit_stream_;
		std::vector<std::vector<std::uint8_t>> perm_table_;
	};

	// Registers pass.
	const llvm::RegisterPass<BlockWatermarkPass> pass_registry =
	{
		"block-wm",
		"Watrermarking pass by code layout methods",
	};
}
