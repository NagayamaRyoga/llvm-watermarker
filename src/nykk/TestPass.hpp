#ifndef INCLUDE_NYKK_TESTPASS_HPP
#define INCLUDE_NYKK_TESTPASS_HPP

#include <llvm/Pass.h>

namespace nykk
{
	class TestPass
		: public llvm::FunctionPass
	{
	public:
		static char ID; // For LLVM.

		explicit TestPass();

		// Uncopyable, unmovable.
		TestPass(const TestPass&) =delete;
		TestPass(TestPass&&) =delete;

		TestPass& operator=(const TestPass&) =delete;
		TestPass& operator=(TestPass&&) =delete;

		~TestPass() =default;

		/**
		 * @brief      Initialization before pass is run.
		 *
		 * @param      module  Reference of the module.
		 *
		 * @return     ?
		 */
		bool doInitialization(llvm::Module& module) override;

		/**
		 * @brief      Finalization after pass is run.
		 *
		 * @param      module  Reference of the module.
		 *
		 * @return     ?
		 */
		bool doFinalization(llvm::Module& module) override;

		/**
		 * @brief      Processes functions.
		 *
		 * @param      func  Reference to the function.
		 *
		 * @return     `true` if the function was changed.
		 */
		bool runOnFunction(llvm::Function& func) override;
	};
}

#endif
