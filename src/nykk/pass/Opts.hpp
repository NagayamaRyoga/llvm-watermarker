#ifndef INCLUDE_NYKK_PASS_OPTS_HPP
#define INCLUDE_NYKK_PASS_OPTS_HPP

#include <llvm/Support/CommandLine.h>

namespace nykk::pass
{
	template <typename Integer>
	struct RangeOptValue
	{
		Integer value;
	};

	/**
	 * @brief      Command line option analyzer.
	 *
	 * @tparam     Integer  Integer type.
	 * @tparam     Min      Minimum value to accept.
	 * @tparam     Max      Maximum value to accept.
	 */
	template <typename Integer, Integer Min, Integer Max>
	class RangeOptParser
		: public llvm::cl::parser<RangeOptValue<Integer>>
	{
		static_assert(std::is_integral_v<Integer>);

	public:
		using llvm::cl::parser<RangeOptValue<Integer>>::parser;

		// Returns `true` on error.
		bool parse(llvm::cl::Option& o, llvm::StringRef arg_name, llvm::StringRef arg, RangeOptValue<Integer>& value)
		{
			if (arg.getAsInteger(0, value.value) || value.value < Min || Max < value.value)
			{
				return o.error("invalid argument '" + arg_name + "=" + arg + "'");
			}

			return false;
		}
	};

	inline const llvm::cl::opt<std::string> watermark_opt
	{
		"watermark",
		llvm::cl::desc("Watermark (32bit)"),
		llvm::cl::value_desc("watermark"),
		llvm::cl::Required,
	};
}

#endif
