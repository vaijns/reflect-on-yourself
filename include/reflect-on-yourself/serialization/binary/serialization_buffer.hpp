#ifndef H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_SERIALIZATION_BUFFER
#define H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_SERIALIZATION_BUFFER

#include <cstring>
#include <span>
#include <cstddef>
#include <array>
#include <optional>
#include <type_traits>
#include <concepts>
#include <memory>
#include <vector>
#include <algorithm>
#include <ranges>

#include "./data_type.hpp"

namespace roy::serialization::detail{
	class serialization_bridge;
}

namespace roy::serialization::detail{
	class binary_serializer;
	class serialization_read_buffer;

	class serialization_write_buffer{
	public:
		friend class roy::serialization::detail::binary_serializer;
		friend class roy::serialization::detail::serialization_read_buffer;

		constexpr serialization_write_buffer(std::size_t size)
			: m_memory{std::make_unique<std::byte[]>(size)},
			m_capacity{size},
			m_size{0}{

		}
	private:
		template<typename T>
		constexpr auto copy_bytes(const T* t, std::size_t byte_count)
			-> bool{
			if(m_capacity < (m_size + byte_count)){
				return false;
			}

			std::memcpy(std::next(m_memory.get(), m_size), t, byte_count);
			m_size = m_size + byte_count;

			return true;
		}

		template<std::ranges::forward_range R>
			requires(std::ranges::sized_range<R> and std::is_same_v<std::ranges::range_value_t<R>, std::byte>)
		constexpr auto write_bytes(
			R&& bytes
		) noexcept -> bool{
			if(m_capacity < (m_size + std::ranges::size(bytes))){
				return false;
			}

			std::copy(std::begin(bytes), std::end(bytes), std::next(m_memory.get(), m_size));
			m_size = m_size + std::ranges::size(bytes);

			return true;
		}

		template<typename CharT>
		constexpr auto add_string_capacity(
			std::size_t amount
		) noexcept -> void{
			if constexpr(std::same_as<char, CharT>){
				m_strings_capacity += amount;
			} else if constexpr(std::same_as<wchar_t, CharT>){
				m_wstrings_capacity += amount;
			} else if constexpr(std::same_as<char8_t, CharT>){
				m_u8strings_capacity += amount;
			} else if constexpr(std::same_as<char16_t, CharT>){
				m_u16strings_capacity += amount;
			} else if constexpr(std::same_as<char32_t, CharT>){
				m_u32strings_capacity += amount;
			}
		}

		std::unique_ptr<std::byte[]> m_memory;
		std::size_t m_capacity;
		std::size_t m_size;

		std::size_t m_strings_capacity{0};
		std::size_t m_wstrings_capacity{0};
		std::size_t m_u8strings_capacity{0};
		std::size_t m_u16strings_capacity{0};
		std::size_t m_u32strings_capacity{0};
	};

	class serialization_read_buffer_view;

	class serialization_read_buffer{
	public:
		friend class roy::serialization::detail::serialization_bridge;
		friend class roy::serialization::detail::serialization_read_buffer_view;

		constexpr serialization_read_buffer(serialization_write_buffer&& buffer)
			: m_memory{std::move(buffer.m_memory)},
			m_capacity{buffer.m_size},
			m_strings_capacity{buffer.m_strings_capacity},
			m_wstrings_capacity{buffer.m_wstrings_capacity},
			m_u8strings_capacity{buffer.m_u8strings_capacity},
			m_u16strings_capacity{buffer.m_u16strings_capacity},
			m_u32strings_capacity{buffer.m_u32strings_capacity}{

		}
	private:
		std::unique_ptr<std::byte[]> m_memory;
		std::size_t m_capacity;

		std::size_t m_strings_capacity;
		std::size_t m_wstrings_capacity;
		std::size_t m_u8strings_capacity;
		std::size_t m_u16strings_capacity;
		std::size_t m_u32strings_capacity;
	};

	class serialization_read_buffer_view{
	public:
		friend class roy::serialization::detail::serialization_bridge;

		constexpr serialization_read_buffer_view(serialization_read_buffer& buffer)
			: m_memory{std::span{buffer.m_memory.get(), buffer.m_capacity}},
			m_remaining_size{buffer.m_capacity}{

		}
	private:
		constexpr auto read_bytes(
			std::size_t amount
		) noexcept -> std::optional<std::span<std::byte>>{
			if(m_remaining_size < amount)
				return std::nullopt;

			m_remaining_size = m_remaining_size - amount;
			return m_memory.subspan(m_remaining_size, amount);
		}

		constexpr auto read_data_info(
		) noexcept -> std::optional<roy::serialization::detail::data_info>{
			return read_bytes(1).transform([](std::span<std::byte> bytes){
				return std::bit_cast<roy::serialization::detail::data_info>(bytes[0]);
			});
		}

		std::span<std::byte> m_memory;
		std::size_t m_remaining_size;
	};
}

#endif
