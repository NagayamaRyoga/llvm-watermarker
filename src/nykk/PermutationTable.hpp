#ifndef INCLUDE_NYKK_PERMUTATIONTABLE_HPP
#define INCLUDE_NYKK_PERMUTATIONTABLE_HPP

#include <cassert>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <vector>

#include <iostream>

namespace nykk
{
	/**
	 * @brief      Compute factorial `x!`.
	 *
	 * @param[in]  x
	 *
	 * @tparam     Integer    Integer type.
	 * @tparam     <unnamed>
	 *
	 * @return     `x!`.
	 */
	template <typename Integer, std::enable_if_t<std::is_integral_v<Integer>, std::nullptr_t> = nullptr>
	[[nodiscard]]
	constexpr Integer factorial(Integer x)
	{
		Integer n = 1;

		for (Integer i = 1; i <= x; ++i)
		{
			n *= i;
		}

		return n;
	}

	/**
	 * @brief      Compute a permutation table of sequence [0, n).
	 *
	 * @param[in]  n     Size of a number sequence.
	 *
	 * @return     Pre-computed permutation table.
	 */
	[[nodiscard]]
	inline std::vector<std::vector<std::uint8_t>> create_permutation_table(std::size_t n)
	{
		assert(n < 13 && "too large `n` to compute permutation table");

		auto table = std::vector<std::vector<std::uint8_t>> {};
		table.reserve(factorial(n));

		// [0, n)
		auto v = std::vector<std::uint8_t>(n);
		std::iota(std::begin(v), std::end(v), 0);

		do
		{
			table.emplace_back(v);
		} while (std::next_permutation(std::begin(v), std::end(v)));

		return table;
	}
}

#endif
