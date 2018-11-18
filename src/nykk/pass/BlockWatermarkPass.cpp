#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include "../PermutationTable.hpp"

namespace
{
	/**
	 * @brief      Command line option analyzer.
	 *
	 * @tparam     Integer  Integer type.
	 * @tparam     Min      Minimum value to accept.
	 * @tparam     Max      Maximum value to accept.
	 */
	template <typename Integer, Integer Min, Integer Max>
	class RangeOptParser
		: public llvm::cl::parser<Integer>
	{
		static_assert(std::is_integral_v<Integer>);

	public:
		using llvm::cl::parser<Integer>::parser;

		// Returns `true` on error.
		bool parse(llvm::cl::Option& o, [[maybe_unused]] llvm::StringRef arg_name, llvm::StringRef arg, Integer& val)
		{
			try
			{
				const auto v = [](llvm::StringRef s)
				{
					if constexpr (std::is_unsigned_v<Integer>)
					{
						return std::stoull(s.str());
					}
					else
					{
						return std::stoll(s.str());
					}
				}(arg);

				if (Min <= v && v <= Max)
				{
					val = static_cast<Integer>(v);
					return false;
				}
			}
			catch ([[maybe_unused]] const std::exception& e)
			{
			}

			o.error("invalid argument '" + arg_name + "=" + arg + "'");

			return true;
		}
	};

	const llvm::cl::opt<std::size_t, false, RangeOptParser<std::size_t, 2, 10>> partition_opt
	{
		"partition",
		llvm::cl::desc("Block partition number (2 ~ 10 default 7)"),
		llvm::cl::value_desc("size"),
		llvm::cl::init(7),
	};

	const llvm::cl::opt<int> watermark_opt
	{
		"watermark",
		llvm::cl::desc("Watermark (32bit)"),
		llvm::cl::value_desc("watermark"),
		llvm::cl::Required,
	};

	/**
	 * @brief      Watermarking pass by code layout methods.
	 */
	class BlockWatermarkPass
		: public llvm::FunctionPass
	{
	public:
		static char ID; // Needed for LLVM.

		/**
		 * @brief      Constructor.
		 */
		explicit BlockWatermarkPass()
			: FunctionPass(ID)
			, module_name_()
			, perm_table_()
			, bit_pos_(0)
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
			perm_table_ = nykk::create_permutation_table(partition_opt);
			bit_pos_ = 0;

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
				<< "[BlockWatermarker - '" << func.getName() << "' in " << module_name_ << "] Basic blocks: " << func.size() << "\n";

			if (func.size() <= partition_opt)
			{
				llvm::errs()
					<< "    function '" << func.getName() << "' is too small to watermark" << "\n";

				return false;
			}

			// Table of {log2 x! | x < 13}
			constexpr auto possible_embedding_bits = std::array<std::size_t, 13>
			{
				0, 0, 1, 2, 4, 6, 12, 15, 18, 21, 25, 28,
			};

			const auto blocks = std::vector<std::reference_wrapper<llvm::BasicBlock>>(std::begin(func), std::end(func));
			auto last_block = &func.getEntryBlock();

			// Embeds watermark.
			std::size_t num_embedded_bits = 0;
			std::size_t block_index = 1; // Without entry block.

			const auto bit_mask = (1 << possible_embedding_bits[partition_opt]) - 1;

			for (; block_index + partition_opt <= blocks.size(); block_index += partition_opt)
			{
				// Part of watermark to embed.
				const auto data = (watermark_opt >> bit_pos_) & bit_mask;

				// Shuffles each `partition_opt` blocks.
				for (std::size_t i = 0; i < partition_opt; i++)
				{
					const auto index = block_index + perm_table_.at(data).at(i);

					blocks[index].get().moveAfter(last_block);
					last_block = &blocks[index].get();
				}

				num_embedded_bits += possible_embedding_bits[partition_opt];
				bit_pos_ += possible_embedding_bits[partition_opt];
				bit_pos_ %= sizeof(std::uint32_t) * 8;
			}

			// Inserts rest blocks.
			for (; block_index < blocks.size(); block_index++)
			{
				blocks[block_index].get().moveAfter(last_block);
				last_block = &blocks[block_index].get();
			}

			llvm::errs()
				<< "    " << num_embedded_bits << "bit watermark embedded in function '" << func.getName() << "'" << "\n";

			return true;
		}

	private:
		std::string module_name_;
		std::vector<std::vector<std::uint8_t>> perm_table_;
		std::uint32_t bit_pos_;
	};

	char BlockWatermarkPass::ID;

	// Registers pass.
	const llvm::RegisterPass<BlockWatermarkPass> pass_registry =
	{
		"block-wm",
		"Watrermarking pass by code layout methods",
	};
}
