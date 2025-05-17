#ifndef H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_DESERIALIZER
#define H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_DESERIALIZER

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

#include "../serialization_strategy.hpp"

namespace roy::serialization::detail{
	enum struct data_type : std::uint8_t{
		unit = 0,

		null_value = 1,

		nullopt = 2,

		boolean = 3,

		byte = 4,

		uint8 = 5,
		uint16 = 6,
		uint32 = 7,
		uint64 = 8,
#ifdef __SIZEOF_INT128__
		uint128 = 9,
#endif

		int8 = 10,
		int16 = 11,
		int32 = 12,
		int64 = 13,
#ifdef __SIZEOF_INT128__
		int128 = 14,
#endif

		float32 = 15,
		float64 = 16,
		float128 = 17,

		character = 18,
		wcharacter = 19,
		u8character = 20,
		u16character = 21,
		u32character = 22,

		string = 23,
		wstring = 24,
		u8string = 25,
		u16string = 26,
		u32string = 27,

		unix_timestamp_days = 28,
		unix_timestamp_hours = 29,
		unix_timestamp_minutes = 30,
		unix_timestamp_seconds = 31,
		unix_timestamp_milliseconds = 32,
		unix_timestamp_microseconds = 33,
		unix_timestamp_nanoseconds = 34,

		duration_years = 35,
		duration_months = 36,
		duration_weeks = 37,
		duration_days = 38,
		duration_hours = 39,
		duration_minutes = 40,
		duration_seconds = 41,
		duration_milliseconds = 42,
		duration_microseconds = 43,
		duration_nanoseconds = 44,

		array = 45,
		object = 46,

		max_value = 63
	};

	enum struct size_bytes : std::uint8_t{
		one_byte = 0b00,
		two_bytes = 0b01,
		three_bytes = 0b10,
		four_bytes = 0b11
	};

	struct data_info{
		roy::serialization::detail::size_bytes size_bytes : 2;
		roy::serialization::detail::data_type type : 6;
	};

	template<typename T>
	struct get_data_type;

	template<typename T>
	inline constexpr roy::serialization::detail::data_type get_data_type_v{roy::serialization::detail::get_data_type<T>::value};

	template<> struct get_data_type<std::nullopt_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::nullopt}; };

	template<> struct get_data_type<bool>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::boolean}; };

	template<> struct get_data_type<std::int8_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::int8}; };
	template<> struct get_data_type<std::int16_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::int16}; };
	template<> struct get_data_type<std::int32_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::int32}; };
	template<> struct get_data_type<std::int64_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::int64}; };
	template<> struct get_data_type<__int128_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::int128}; };

	template<> struct get_data_type<std::uint8_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::uint8}; };
	template<> struct get_data_type<std::uint16_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::uint16}; };
	template<> struct get_data_type<std::uint32_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::uint32}; };
	template<> struct get_data_type<std::uint64_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::uint64}; };
	template<> struct get_data_type<__uint128_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::uint128}; };

	template<> struct get_data_type<float>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::float32}; };
	template<> struct get_data_type<double>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::float64}; };
	template<> struct get_data_type<long double>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::float128}; };

	template<> struct get_data_type<char>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::character}; };
	template<> struct get_data_type<wchar_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::wcharacter}; };
	template<> struct get_data_type<char8_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u8character}; };
	template<> struct get_data_type<char16_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u16character}; };
	template<> struct get_data_type<char32_t>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u32character}; };

	template<> struct get_data_type<std::string>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::string}; };
	template<> struct get_data_type<std::wstring>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::wstring}; };
	template<> struct get_data_type<std::u8string>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u8string}; };
	template<> struct get_data_type<std::u16string>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u16string}; };
	template<> struct get_data_type<std::u32string>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u32string}; };

	template<> struct get_data_type<std::string_view>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::string}; };
	template<> struct get_data_type<std::wstring_view>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::wstring}; };
	template<> struct get_data_type<std::u8string_view>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u8string}; };
	template<> struct get_data_type<std::u16string_view>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u16string}; };
	template<> struct get_data_type<std::u32string_view>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u32string}; };

	template<> struct get_data_type<const char*>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::string}; };
	template<> struct get_data_type<const wchar_t*>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::wstring}; };
	template<> struct get_data_type<const char8_t*>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u8string}; };
	template<> struct get_data_type<const char16_t*>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u16string}; };
	template<> struct get_data_type<const char32_t*>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::u32string}; };

	template<typename E> requires std::is_enum_v<E> struct get_data_type<E>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::get_data_type_v<std::underlying_type_t<E>>}; };

	template<std::same_as<std::nullopt_t> T> struct get_data_type<T>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::nullopt}; };
	template<std::same_as<std::nullptr_t> T> struct get_data_type<T>{
		static constexpr roy::serialization::detail::data_type value{roy::serialization::detail::data_type::null_value}; };
}

#endif
