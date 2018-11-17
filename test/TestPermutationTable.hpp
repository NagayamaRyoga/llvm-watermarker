#ifndef INCLUDE_TEST_TESTPERMUTATIONTABLE_HPP
#define INCLUDE_TEST_TESTPERMUTATIONTABLE_HPP

#include "../src/nykk/PermutationTable.hpp"

template <typename T, typename U>
[[nodiscard]]
inline bool range_test(std::initializer_list<T> expected, U&& actual)
{
	return std::equal(std::begin(expected), std::end(expected), std::begin(actual), std::end(actual));
}

inline void run_test_permutation_table()
{
	static_assert(nykk::factorial(0) == 1);
	static_assert(nykk::factorial(1) == 1);
	static_assert(nykk::factorial(2) == 2);
	static_assert(nykk::factorial(5) == 120);

	const auto table = nykk::create_permutation_table(3);

	assert(table.size() == 6);

	assert(range_test({0, 1, 2}, table[0]));
	assert(range_test({0, 2, 1}, table[1]));
	assert(range_test({1, 0, 2}, table[2]));
	assert(range_test({1, 2, 0}, table[3]));
	assert(range_test({2, 0, 1}, table[4]));
	assert(range_test({2, 1, 0}, table[5]));
}

#endif
