#include <array>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

#include <zlib.h>

namespace
{
	void compression_test(const std::vector<std::byte>& raw_test_data, std::vector<std::byte>& buffer, int level)
	{
		::z_stream zs = {};
		zs.next_in = reinterpret_cast<Bytef*>(const_cast<std::byte*>(raw_test_data.data())); // Never rewrited.
		zs.avail_in = raw_test_data.size();
		zs.next_out = reinterpret_cast<Bytef*>(buffer.data());
		zs.avail_out = buffer.size();

		::deflateInit2(&zs, level, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
		::deflate(&zs, Z_FINISH);
		::deflateEnd(&zs);
	}

	void decompression_test(const std::vector<std::byte>& compressed_test_data, std::vector<std::byte>& buffer)
	{
		::z_stream zs = {};
		zs.next_in = reinterpret_cast<Bytef*>(const_cast<std::byte*>(compressed_test_data.data())); // Never rewrited.
		zs.avail_in = compressed_test_data.size();
		zs.next_out = reinterpret_cast<Bytef*>(buffer.data());
		zs.avail_out = buffer.size();

		::inflateInit2(&zs, -MAX_WBITS);
		::inflate(&zs, Z_FINISH);
		::inflateEnd(&zs);
	}

	template <typename F>
	std::chrono::high_resolution_clock::duration measure(F&& f)
	{
		const auto start = std::chrono::high_resolution_clock::now();
		f();
		const auto end = std::chrono::high_resolution_clock::now();

		return end - start;
	}
}

int main()
{
	// 200MB random array.
	const auto raw_test_data = []()
	{
		constexpr std::size_t size = 200 * 1024 * 1024;
		auto data = std::vector<std::byte>(size);

		auto rand = std::mt19937 {}; // Always use the default seed to get the same result.

		using ByteBase = std::underlying_type_t<std::byte>; // unsigned char
		auto dist = std::uniform_int_distribution<ByteBase> { ByteBase(0), ByteBase(-1) };

		for (auto& x : data)
		{
			x = std::byte { dist(rand) };
		}

		return data;
	}();

	// Compressed `test_data`.
	const auto compressed_test_data = [&]()
	{
		auto data = std::vector<std::byte>(::compressBound(raw_test_data.size()));

		::z_stream zs = {};
		zs.next_in = reinterpret_cast<Bytef*>(const_cast<std::byte*>(raw_test_data.data())); // Never rewrited.
		zs.avail_in = raw_test_data.size();
		zs.next_out = reinterpret_cast<Bytef*>(data.data());
		zs.avail_out = data.size();

		::deflateInit2(&zs, 9, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
		::deflate(&zs, Z_FINISH);
		::deflateEnd(&zs);

		return std::vector<std::byte>(std::begin(data), std::end(data));
	}();

	std::cout << "compression[us],decompression[us]" << std::endl;

	constexpr std::size_t n = 100;

	auto output_buffer = std::vector<std::byte>(::compressBound(raw_test_data.size()));

	for (std::size_t i = 0; i < n; i++)
	{
		const auto compression_time = measure([&]() {
			compression_test(raw_test_data, output_buffer, 9);
		});

		const auto decompression_time = measure([&]() {
			decompression_test(compressed_test_data, output_buffer);
		});

		std::cout
			<< std::chrono::duration_cast<std::chrono::microseconds>(compression_time).count() << ","
			<< std::chrono::duration_cast<std::chrono::microseconds>(decompression_time).count() << std::endl;
	}
}
