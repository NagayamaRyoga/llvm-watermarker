#ifndef INCLUDE_TEST_TESTCIRCULARBITSTREAM_HPP
#define INCLUDE_TEST_TESTCIRCULARBITSTREAM_HPP

#include "../src/nykk/CircularBitStream.hpp"

inline void run_test_circular_bit_stream()
{
	auto s = nykk::CircularBitStream::from_string(u8"\xab\xcd\xef\x98");

	assert(s.size_bytes() == 4);
	assert(s.size_bits() == 32);

	assert(s.read_bit() == true);
	assert(s.read_bit() == true);
	assert(s.read_bit() == false);
	assert(s.pos_bit() == 3);

	assert(s.read(17) == 0x1f9b5);
	assert(s.pos_bit() == 20);

	assert(s.read(16) == 0xb98e);
	assert(s.pos_bit() == 4);

	assert(s.read(64) == 0xb98efcdab98efcda);
	assert(s.pos_bit() == 4);
}

#endif
