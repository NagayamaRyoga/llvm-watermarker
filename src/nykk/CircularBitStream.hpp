#ifndef INCLUDE_NYKK_CIRCULARBITSTREAM_HPP
#define INCLUDE_NYKK_CIRCULARBITSTREAM_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace nykk
{
	class CircularBitStream
	{
	public:
		static CircularBitStream from_string(std::string_view s)
		{
			return CircularBitStream { reinterpret_cast<const std::byte*>(s.data()), s.size() };
		}

		explicit CircularBitStream(const std::byte* pointer, std::size_t length)
			: data_(pointer, pointer + length)
			, bit_pos_(0)
		{
			assert(pointer != nullptr || length == 0);
		}

		// Uncopyable, movable.
		CircularBitStream(const CircularBitStream&) =delete;
		CircularBitStream(CircularBitStream&&) =default;

		CircularBitStream& operator=(const CircularBitStream&) =delete;
		CircularBitStream& operator=(CircularBitStream&&) =default;

		~CircularBitStream() =default;

		[[nodiscard]]
		const std::byte* data() const noexcept
		{
			return data_.data();
		}

		[[nodiscard]]
		std::size_t size_bytes() const noexcept
		{
			return data_.size();
		}

		[[nodiscard]]
		std::size_t size_bits() const noexcept
		{
			return size_bytes() * 8;
		}

		[[nodiscard]]
		std::size_t pos_bit() const noexcept
		{
			return bit_pos_;
		}

		std::uint64_t read(std::size_t size_bits)
		{
			assert(size_bits <= 64);

			std::uint64_t data = 0;

			for (std::size_t i = 0; i < size_bits; i++)
			{
				data |= read_bit() ? std::uint64_t {1} << i : 0;
			}

			return data;
		}

		bool read_bit()
		{
			if (size_bits() == 0)
			{
				return false;
			}

			const std::byte value = data_[bit_pos_ >> 3] >> (bit_pos_ & 0x7);

			if (++bit_pos_ >= size_bits())
			{
				bit_pos_ = 0;
			}

			return std::to_integer<bool>(value & std::byte {1});
		}

	private:
		std::vector<std::byte> data_;
		std::size_t bit_pos_;
	};
}

#endif
