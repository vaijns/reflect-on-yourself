#ifndef H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_SIZE_CALCULATOR
#define H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_SIZE_CALCULATOR

#include <cstring>
#include <span>
#include <cstddef>
#include <array>
#include <optional>
#include <type_traits>
#include <concepts>
#include <memory>
#include <vector>
#include <expected>
#include <cstdint>

#include "./serialization_buffer.hpp"
#include "./data_type.hpp"
#include "../serialization_strategy.hpp"

namespace roy::serialization::detail{
	namespace detail{
		template<typename T>
		struct is_string_literal : std::false_type{};

		template<std::size_t N, typename CharT>
		struct is_string_literal<const CharT (&)[N]> : std::true_type{};

		template<typename T>
		inline constexpr bool is_string_literal_v{roy::serialization::detail::detail::is_string_literal<T>::value};

		template<typename T>
		concept string_literal = roy::serialization::detail::detail::is_string_literal_v<T>;
	}

	class size_calculator{
	public:
		friend class serializer;

		template<std::integral I>
		static constexpr auto calculate_size(
		)  noexcept -> std::size_t{
			return sizeof(I);
		}

		template<std::floating_point F>
		static constexpr auto calculate_size(
		)  noexcept -> std::size_t{
			return sizeof(F);
		}

		template<std::same_as<roy::serialization::detail::data_info> D>
		static constexpr auto calculate_size(
		) noexcept -> std::size_t{
			return sizeof(D);
		}

		template<typename E> requires(std::is_enum_v<E>)
		static constexpr auto calculate_size(
		)  noexcept -> std::size_t{
			return roy::serialization::detail::size_calculator::calculate_size<std::underlying_type_t<E>>();
		}

		template<typename T>
		static constexpr auto calculate_size(
			const std::optional<T>& data
		) noexcept -> std::size_t{
			if(not data.has_value())
				return 0;

			return roy::serialization::detail::size_calculator::calculate_size(data.value());
		}

		static constexpr auto max_bit_value(std::uint8_t bits)
			-> std::size_t{
			if(bits == 0)
				return 0;

			if(bits > (sizeof(std::size_t) * 8))
				return 0;

			if(bits == (sizeof(std::size_t) * 8))
				return std::numeric_limits<size_t>::max();

			return (1ULL << bits) - 1;
		}

		static constexpr auto count_size_bytes(
			std::size_t size
		) noexcept -> roy::serialization::detail::size_bytes{
			if(size <= std::numeric_limits<std::uint8_t>::max())
				return roy::serialization::detail::size_bytes::one_byte;

			if(size <= std::numeric_limits<std::uint16_t>::max())
				return roy::serialization::detail::size_bytes::two_bytes;

			if(size <= std::numeric_limits<std::uint32_t>::max())
				return roy::serialization::detail::size_bytes::three_bytes;

			if(size <= std::numeric_limits<std::uint64_t>::max())
				return roy::serialization::detail::size_bytes::four_bytes;

			return roy::serialization::detail::size_bytes::four_bytes;
		}

		static constexpr auto calculate_length_size(
			std::size_t size
		) noexcept -> std::size_t{
			switch(count_size_bytes(size)){
			case roy::serialization::detail::size_bytes::one_byte:
				return 1;
			case roy::serialization::detail::size_bytes::two_bytes:
				return 2;
			case roy::serialization::detail::size_bytes::three_bytes:
				return 3;
			case roy::serialization::detail::size_bytes::four_bytes: [[fallthrough]];
			default:
				return 4;
			}
		}

		template<typename CharT>
		static constexpr auto calculate_size(
			const std::basic_string<CharT, std::char_traits<CharT>>& data
		) noexcept -> std::size_t{
			return (sizeof(CharT) * data.size()) + roy::serialization::detail::size_calculator::calculate_length_size(data.size());
		}

		template<typename CharT>
		static constexpr auto calculate_size(
			const std::basic_string_view<CharT, std::char_traits<CharT>>& data
		) noexcept -> std::size_t{
			return (sizeof(CharT) * data.size()) + roy::serialization::detail::size_calculator::calculate_length_size(data.size());
		}

		template<typename CharT>
		static constexpr auto null_terminated_string_length(
			const CharT* data
		) noexcept -> std::size_t{
			std::size_t size{0};

			if constexpr (std::same_as<char, CharT>){
				while(data[size] != '\0')
					size++;
			} else if constexpr (std::same_as<wchar_t, CharT>){
				while(data[size] != L'\0')
					size++;
			} else if constexpr (std::same_as<char8_t, CharT>){
				while(data[size] != u8'\0')
					size++;
			} else if constexpr (std::same_as<char16_t, CharT>){
				while(data[size] != u'\0')
					size++;
			} else if constexpr (std::same_as<char32_t, CharT>){
				while(data[size] != U'\0')
					size++;
			}

			return size;
		}

		template<typename CharT>
		static constexpr auto calculate_size(
			const CharT* data
		) noexcept -> std::size_t{
			std::size_t size{roy::serialization::detail::size_calculator::null_terminated_string_length(data)};
			return (sizeof(CharT) * size) +
				roy::serialization::detail::size_calculator::calculate_length_size(size);
		}

		template<typename CharT, std::size_t N>
		static constexpr auto calculate_string_literal_size(
			const CharT (&)[N]
		) noexcept -> std::size_t{
			return (sizeof(CharT) * (N - 1)) + roy::serialization::detail::size_calculator::calculate_length_size(N - 1);
		}

		template<roy::serialization::detail::detail::string_literal Str>
		static constexpr auto calculate_size(
			Str data
		) noexcept -> std::size_t{
			return calculate_string_literal_size(data);
		}

		template<typename CharT, std::size_t N>
		static constexpr auto calculate_size(
			const roy::util::basic_inplace_string<CharT, N, std::char_traits<CharT>>& data
		) noexcept -> std::size_t{
			return (sizeof(CharT) * data.size()) + roy::serialization::detail::size_calculator::calculate_length_size(N - 1);
		}

		template<typename T> requires requires(T t){ roy::serialization::detail::size_calculator::calculate_size<std::remove_cvref_t<T>>(); }
		static constexpr auto calculate_size(T) noexcept -> std::size_t{
			return roy::serialization::detail::size_calculator::calculate_size<std::remove_cvref_t<T>>();
		}

		template<roy::serialization_strategy<roy::serialization::strategy::as_object> T>
			requires requires(T t) { roy::field_count_of<T>(); roy::reflection_of<T>::name(); }
		static constexpr auto calculate_size(
			const T& data
		) noexcept -> std::size_t{
			std::size_t total_size{0};
			total_size += roy::serialization::detail::size_calculator::calculate_size(roy::reflection_of<T>::name()); // object name
			total_size += calculate_length_size(roy::field_count_of<T>()); // property count
			total_size += (roy::field_count_of<T>() * roy::serialization::detail::size_calculator::calculate_size<roy::serialization::detail::data_type>()); // property types
			[&]<std::size_t... Is>(std::index_sequence<Is...>){
				total_size += (roy::serialization::detail::size_calculator::calculate_size(roy::nth_field_name_of<Is, T>()) + ...); // property name
				total_size += (roy::serialization::detail::size_calculator::calculate_size(roy::nth_field_value<Is>(data)) + ...); // property value
			}(std::make_index_sequence<roy::field_count_of<T>()>{});

			return total_size;
		}

		template<roy::serialization_strategy<roy::serialization::strategy::as_array> T>
			requires std::ranges::forward_range<T>
		static constexpr auto calculate_size(
			const T& data
		) noexcept -> std::size_t{
			std::size_t total_size{0};
			total_size += calculate_length_size(std::size(data)); // values count
			total_size += (roy::serialization::detail::size_calculator::calculate_size<roy::serialization::detail::data_type>()); // values type
			for(const auto& value : data){
				total_size += roy::serialization::detail::size_calculator::calculate_size(value); // values
			}

			return total_size;
		}
	};
}

#endif
