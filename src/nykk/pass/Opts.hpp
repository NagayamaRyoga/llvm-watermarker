#ifndef INCLUDE_NYKK_PASS_OPTS_HPP
#define INCLUDE_NYKK_PASS_OPTS_HPP

#include <llvm/Support/CommandLine.h>

namespace nykk::pass
{
	const llvm::cl::opt<int> watermark_opt
	{
		"watermark",
		llvm::cl::desc("Watermark (32bit)"),
		llvm::cl::value_desc("watermark"),
		llvm::cl::Required,
	};
}

#endif
