#ifndef H_REFLECT_ON_YOURSELF_SERIALIZATION_SERIALIZATION_SINK
#define H_REFLECT_ON_YOURSELF_SERIALIZATION_SERIALIZATION_SINK

#include <utility>
#include <concepts>
#include <type_traits>
#include <expected>
#include <string_view>
#include <span>
#include <array>
#include <format>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <print>

#include "./binary/data_type.hpp"
#include "./binary/serialization_buffer.hpp"
#include "./binary/binary_serializer.hpp"

namespace roy::serialization::serialization_types{
	//using duration_nanoseconds = std::chrono::duration<std::int64_t, std::ratio<std::chrono::nanoseconds::period::num>>;
	using duration_nanoseconds = std::chrono::duration<std::int64_t, std::nano>;
	//using duration_microseconds = std::chrono::duration<std::int64_t, std::ratio<std::chrono::microseconds::period::num>>;
	using duration_microseconds = std::chrono::duration<std::int64_t, std::micro>;
	//using duration_milliseconds = std::chrono::duration<std::int64_t, std::ratio<std::chrono::milliseconds::period::num>>;
	using duration_milliseconds = std::chrono::duration<std::int64_t, std::milli>;
	//using duration_seconds = std::chrono::duration<std::int64_t, std::ratio<std::chrono::seconds::period::num>>;
	using duration_seconds = std::chrono::duration<std::int64_t, std::ratio<1>>;
	using duration_minutes = std::chrono::duration<std::int64_t, std::ratio<std::chrono::minutes::period::num>>;
	using duration_hours = std::chrono::duration<std::int64_t, std::ratio<std::chrono::hours::period::num>>;
	using duration_days = std::chrono::duration<std::int64_t, std::ratio<std::chrono::days::period::num>>;
	using duration_weeks = std::chrono::duration<std::int64_t, std::ratio<std::chrono::weeks::period::num>>;
	using duration_months = std::chrono::duration<std::int64_t, std::ratio<std::chrono::months::period::num>>;
	using duration_years = std::chrono::duration<std::int64_t, std::ratio<std::chrono::years::period::num>>;

	using timestamp_clock = std::chrono::system_clock;

	using days_timestamp = std::chrono::time_point<timestamp_clock, duration_days>;
	using hours_timestamp = std::chrono::time_point<timestamp_clock, duration_hours>;
	using minutes_timestamp = std::chrono::time_point<timestamp_clock, duration_minutes>;
	using seconds_timestamp = std::chrono::time_point<timestamp_clock, duration_seconds>;
	using milliseconds_timestamp = std::chrono::time_point<timestamp_clock, duration_milliseconds>;
	using microseconds_timestamp = std::chrono::time_point<timestamp_clock, duration_microseconds>;
	using nanoseconds_timestamp = std::chrono::time_point<timestamp_clock, duration_nanoseconds>;

	enum struct unit_t;
	inline constexpr roy::serialization::serialization_types::unit_t unit{};

	template<typename T>
	concept boolean = std::same_as<std::remove_cv_t<T>, bool>;

	template<typename T>
	concept signed_integral =
#ifdef __SIZEOF_INT128__
		std::same_as<std::remove_cv_t<T>, __int128_t> or
#endif
		std::same_as<std::remove_cv_t<T>, std::int64_t> or
		std::same_as<std::remove_cv_t<T>, std::int16_t> or
		std::same_as<std::remove_cv_t<T>, std::int32_t> or
		std::same_as<std::remove_cv_t<T>, std::int8_t>;

	template<typename T>
	concept unsigned_integral =
#ifdef __SIZEOF_INT128__
		std::same_as<std::remove_cv_t<T>, __uint128_t> or
#endif
		std::same_as<std::remove_cv_t<T>, std::uint64_t> or
		std::same_as<std::remove_cv_t<T>, std::uint16_t> or
		std::same_as<std::remove_cv_t<T>, std::uint32_t> or
		std::same_as<std::remove_cv_t<T>, std::uint8_t>;

	template<typename T>
	concept integral =
		roy::serialization::serialization_types::signed_integral<T> or
		roy::serialization::serialization_types::unsigned_integral<T>;

	template<typename T>
	concept floating_point =
		std::same_as<std::remove_cv_t<T>, long double> or
		std::same_as<std::remove_cv_t<T>, double> or
		std::same_as<std::remove_cv_t<T>, float>;

	template<typename T>
	concept character =
		std::same_as<std::remove_cv_t<T>, char32_t> or
		std::same_as<std::remove_cv_t<T>, char16_t> or
		std::same_as<std::remove_cv_t<T>, char8_t> or
		std::same_as<std::remove_cv_t<T>, wchar_t> or
		std::same_as<std::remove_cv_t<T>, char>;
};

namespace roy::serialization{
	template<typename T>
	concept serialization_sink = requires(T t){
		typename T::settings_type;
		typename T::handle_type;
		typename T::char_type;
		typename T::result_type;
		typename T::write_result_type;

		typename T::error_type;
		typename std::formatter<typename T::error_type, char>;
		{ std::format("{}", std::declval<typename T::error_type>()) }
			-> std::convertible_to<std::string>;
		{ std::declval<std::add_lvalue_reference_t<std::ostream>>() << std::declval<typename T::error_type>() }
			-> std::convertible_to<std::add_lvalue_reference_t<std::ostream>>;

		typename T::opening_error_type;
		typename std::formatter<typename T::opening_error_type, char>;
		{ std::format("{}", std::declval<typename T::opening_error_type>()) }
			-> std::convertible_to<std::string>;
		{ std::declval<std::add_lvalue_reference_t<std::ostream>>() << std::declval<typename T::opening_error_type>() }
			-> std::convertible_to<std::add_lvalue_reference_t<std::ostream>>;

		typename T::closing_error_type;
		typename std::formatter<typename T::closing_error_type, char>;
		{ std::format("{}", std::declval<typename T::closing_error_type>()) }
			-> std::convertible_to<std::string>;
		{ std::declval<std::add_lvalue_reference_t<std::ostream>>() << std::declval<typename T::closing_error_type>() }
			-> std::convertible_to<std::add_lvalue_reference_t<std::ostream>>;

		{ T::open(std::declval<std::add_rvalue_reference_t<typename T::settings_type>>()) }
			-> std::convertible_to<std::expected<typename T::handle_type, typename T::opening_error_type>>;

		{ T::write(std::declval<std::add_lvalue_reference_t<typename T::handle_type>>(), std::declval<typename T::char_type>()) }
			-> std::convertible_to<std::expected<typename T::write_result_type, typename T::error_type>>;
		{ T::write(std::declval<std::add_lvalue_reference_t<typename T::handle_type>>(), std::declval<std::span<typename T::char_type>>()) }
			-> std::convertible_to<std::expected<typename T::write_result_type, typename T::error_type>>;

		/*{ T::close(std::declval<std::add_rvalue_reference_t<typename T::handle_type>>()) }
			-> std::convertible_to<std::expected<typename T::result_type, typename T::closing_error_type>>;

		{ T::empty_result(std::declval<std::add_rvalue_reference_t<typename T::handle_type>>()) }
			-> std::convertible_to<typename T::result_type>;*/
		{ T::close(std::declval<std::add_lvalue_reference_t<typename T::handle_type>>()) }
			-> std::convertible_to<std::expected<typename T::result_type, typename T::closing_error_type>>;

		{ T::empty_result(std::declval<std::add_lvalue_reference_t<typename T::handle_type>>()) }
			-> std::convertible_to<typename T::result_type>;
	} and (not roy::serialization::serialization_types::character<typename T::char_type> or requires (T t){
		{ T::write(std::declval<std::add_lvalue_reference_t<typename T::handle_type>>(), std::declval<std::basic_string_view<typename T::char_type>>()) }
			-> std::convertible_to<std::expected<typename T::write_result_type, typename T::error_type>>;
	});

	enum struct structural_type{
		object_start,
		object_end,
		property_start,
		property_end,
		array_start,
		array_end,
		array_value_start,
		array_value_end
	};

	template<typename T, typename Sink>
	concept serializer = requires(T t){
		typename T::char_type;

		typename T::template templated_result_type<Sink>;
		typename std::formatter<typename T::template templated_result_type<Sink>, char>;
		{ std::format("{}", std::declval<typename T::template templated_result_type<Sink>>()) }
			-> std::convertible_to<std::string>;
		{ std::declval<std::add_lvalue_reference_t<std::ostream>>() << std::declval<typename T::template templated_result_type<Sink>>() }
			-> std::convertible_to<std::add_lvalue_reference_t<std::ostream>>;

		typename T::template templated_error_type<Sink>;
		typename std::formatter<typename T::template templated_error_type<Sink>, char>;
		{ std::format("{}", std::declval<typename T::template templated_error_type<Sink>>()) }
			-> std::convertible_to<std::string>;
		{ std::declval<std::add_lvalue_reference_t<std::ostream>>() << std::declval<typename T::template templated_error_type<Sink>>() }
			-> std::convertible_to<std::add_lvalue_reference_t<std::ostream>>;

		typename T::creation_error_type;
		typename std::formatter<typename T::creation_error_type, char>;
		{ std::format("{}", std::declval<typename T::creation_error_type>()) }
			-> std::convertible_to<std::string>;
		{ std::declval<std::add_lvalue_reference_t<std::ostream>>() << std::declval<typename T::creation_error_type>() }
			-> std::convertible_to<std::add_lvalue_reference_t<std::ostream>>;

		typename T::settings_type;
		typename T::state_type;

		{ T::create(
			std::declval<std::add_rvalue_reference_t<typename T::settings_type>>())
		} -> std::convertible_to<std::expected<typename T::state_type, typename T::creation_error_type>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<char>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<wchar_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<char8_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<char16_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<char32_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char>>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<wchar_t>>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char8_t>>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char16_t>>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char32_t>>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char (&)[5]>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const wchar_t (&)[3]>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char8_t (&)[17]>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char16_t (&)[43]>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char32_t (&)[1]>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char*>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const wchar_t*>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char8_t*>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char16_t*>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<const char32_t*>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::unit_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::nullptr_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::nullopt_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<bool>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::byte>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::uint8_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::uint16_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::uint32_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::uint64_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
#ifdef __SIZEOF_INT128__
		//{ T::template serialize<Sink>(
			//std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			//std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			//std::declval<std::uint128_t>())
		//} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<__uint128_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
#endif

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::int8_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::int16_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::int32_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::int64_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
#ifdef __SIZEOF_INT128__
		//{ T::template serialize<Sink>(
			//std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			//std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			//std::declval<std::int128_t>())
		//} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<__int128_t>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
#endif

		//{ T::template serialize<Sink>(
			//std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			//std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			//std::declval<std::float32_t>())
		//} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		//{ T::template serialize<Sink>(
			//std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			//std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			//std::declval<std::float64_t>())
		//} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		//{ T::template serialize<Sink>(
			//std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			//std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			//std::declval<std::float128_t>())
		//} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<float>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<double>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(),
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<long double>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_nanoseconds>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_microseconds>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_milliseconds>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_seconds>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_minutes>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_hours>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_days>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_weeks>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_months>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::duration_years>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::nanoseconds_timestamp>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::microseconds_timestamp>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::milliseconds_timestamp>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::seconds_timestamp>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::minutes_timestamp>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::hours_timestamp>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::serialization_types::days_timestamp>())
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;

		#warning "TODO: create strong aliases for type name, property count, property type, property name, property index, values type, value count and value index"
		{ T::template serialize<Sink, roy::serialization::structural_type::object_start>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char>>(), // type name
			std::declval<std::size_t>()) // property count
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink, roy::serialization::structural_type::object_end>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char>>(), // type name
			std::declval<std::size_t>()) // property count
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink, roy::serialization::structural_type::property_start>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char>>(), // type name
			std::declval<std::size_t>(), // property count
			std::declval<roy::serialization::detail::data_type>(), // property type
			std::declval<std::basic_string_view<char>>(), // property name
			std::declval<std::size_t>()) // property index
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink, roy::serialization::structural_type::property_end>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<std::basic_string_view<char>>(), // type name
			std::declval<std::size_t>(), // property count
			std::declval<roy::serialization::detail::data_type>(), // property type
			std::declval<std::basic_string_view<char>>(), // property name
			std::declval<std::size_t>()) // property index
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink, roy::serialization::structural_type::array_start>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::detail::data_type>(), // values type
			std::declval<std::size_t>()) // value count
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink, roy::serialization::structural_type::array_end>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::detail::data_type>(), // values type
			std::declval<std::size_t>()) // value count
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink, roy::serialization::structural_type::array_value_start>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::detail::data_type>(), // values type
			std::declval<std::size_t>(), // value count
			std::declval<std::size_t>()) // value index
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
		{ T::template serialize<Sink, roy::serialization::structural_type::array_value_end>(
			std::declval<std::add_lvalue_reference_t<typename T::state_type>>(), 
			std::declval<std::add_lvalue_reference_t<typename Sink::handle_type>>(),
			std::declval<roy::serialization::detail::data_type>(), // values type
			std::declval<std::size_t>(), // value count
			std::declval<std::size_t>()) // value index
		} -> std::convertible_to<std::expected<typename T::template templated_result_type<Sink>, typename T::template templated_error_type<Sink>>>;
	} and roy::serialization::serialization_sink<Sink> and std::same_as<typename Sink::char_type, typename T::char_type>;

	class stringstream_sink{
	public:
		struct settings_type{};
		using handle_type = std::stringstream;
		using char_type = char;
		using result_type = std::string;
		struct write_result_type{
			handle_type& handle;
		};

		using error_type = std::string;
		using opening_error_type = std::string;
		using closing_error_type = std::string;

		static constexpr auto open(settings_type&& path) -> std::expected<handle_type, opening_error_type>{
			return handle_type{};
		}

		static constexpr auto write(handle_type& handle, char value) -> std::expected<write_result_type, error_type>{
			handle << value;
			return write_result_type{handle};
		}

		static constexpr auto write(handle_type& handle, std::span<char> value) -> std::expected<write_result_type, error_type>{
			handle << std::string_view{value.data(), value.size()};
			return write_result_type{handle};
		}

		static constexpr auto write(handle_type& handle, std::basic_string_view<char> value) -> std::expected<write_result_type, error_type>{
			handle << value;
			return write_result_type{handle};
		}

		static constexpr auto close(handle_type& handle) -> std::expected<result_type, closing_error_type>{
			return handle.str();
		}

		static constexpr auto empty_result(handle_type& handle) -> result_type{
			return std::string{};
		}
	};

	class file_sink{
	public:
		using settings_type = std::filesystem::path;
		using handle_type = std::fstream;
		using char_type = char;
		using result_type = roy::serialization::serialization_types::unit_t;
		struct write_result_type{
			handle_type& handle;
		};

		using error_type = std::string;
		using opening_error_type = std::string;
		using closing_error_type = std::string;

		static constexpr auto open(settings_type&& path) -> std::expected<handle_type, opening_error_type>{
			return handle_type{path, std::ios::out};
		}

		static constexpr auto write(handle_type& handle, char value) -> std::expected<write_result_type, error_type>{
			handle << value;
			return write_result_type{handle};
		}

		static constexpr auto write(handle_type& handle, std::span<char> value) -> std::expected<write_result_type, error_type>{
			handle << std::string_view{value.data(), value.size()};
			return write_result_type{handle};
		}

		static constexpr auto write(handle_type& handle, std::basic_string_view<char> value) -> std::expected<write_result_type, error_type>{
			handle << value;
			return write_result_type{handle};
		}

		static constexpr auto close(handle_type& handle) -> std::expected<result_type, closing_error_type>{
			handle.close();
			return roy::serialization::serialization_types::unit;
		}

		static constexpr auto empty_result(handle_type& handle) -> result_type{
			handle.close();
			return roy::serialization::serialization_types::unit;
		}
	};

	class size_counter_sink{
	public:
		struct settings_type{};
		struct handle_type{
			std::size_t current_size;
		};
		using char_type = char;
		using result_type = std::size_t;
		struct write_result_type{
			handle_type handle;
		};

		using error_type = std::string;
		using opening_error_type = std::string;
		using closing_error_type = std::string;

		static constexpr auto open(settings_type&&) -> std::expected<handle_type, opening_error_type>{
			return handle_type{
				.current_size{0}
			};
		}

		static constexpr auto write(handle_type& handle, char) -> std::expected<write_result_type, error_type>{
			handle.current_size += 1;
			return write_result_type{handle};
		}

		static constexpr auto write(handle_type& handle, std::span<char> value) -> std::expected<write_result_type, error_type>{
			handle.current_size += value.size();
			return write_result_type{handle};
		}

		static constexpr auto write(handle_type& handle, std::basic_string_view<char> value) -> std::expected<write_result_type, error_type>{
			handle.current_size += value.size();
			return write_result_type{handle};
		}

		static constexpr auto close(handle_type& handle) -> std::expected<result_type, closing_error_type>{
			return handle.current_size;
		}

		static constexpr auto empty_result(handle_type& handle) -> result_type{
			return 0;
		}
	};

	class binary_file_sink{
	public:
		using settings_type = std::filesystem::path;
		using handle_type = std::fstream;
		using char_type = std::byte;
		using result_type = roy::serialization::serialization_types::unit_t;
		struct write_result_type{
			handle_type& handle;
		};

		using error_type = std::string;
		using opening_error_type = std::string;
		using closing_error_type = std::string;

		static constexpr auto open(settings_type&& path) -> std::expected<handle_type, opening_error_type>{
			return handle_type{path, std::ios::out | std::ios::binary};
		}

		static constexpr auto write(handle_type& handle, std::byte value) -> std::expected<write_result_type, error_type>{
			handle << static_cast<std::uint8_t>(value);
			return write_result_type{handle};
		}

		static constexpr auto write(handle_type& handle, std::span<std::byte> value) -> std::expected<write_result_type, error_type>{
			for(std::byte b : value){
				handle << static_cast<std::uint8_t>(b);
			}
			return write_result_type{handle};
		}

		static constexpr auto close(handle_type& handle) -> std::expected<result_type, closing_error_type>{
			handle.close();
			return roy::serialization::serialization_types::unit;
		}

		static constexpr auto empty_result(handle_type& handle) -> result_type{
			handle.close();
			return roy::serialization::serialization_types::unit;
		}
	};

	struct json_settings{};

	class json_serializer{
	public:
		using char_type = char;
		using settings_type = roy::serialization::json_settings;
		struct state_type{};
		using result_type = bool;
		using error_type = std::string;
		using creation_error_type = std::string;

		template<roy::serialization::serialization_sink Sink>
		using templated_result_type = result_type;

		template<roy::serialization::serialization_sink Sink>
		using templated_error_type = error_type;

		static constexpr auto create(settings_type&& settings)
			-> std::expected<state_type, creation_error_type>{
			return state_type{};
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '"');
			Sink::write(handle, value);
			Sink::write(handle, '"');
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			wchar_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write wchar"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char8_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u8char"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char16_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u16char"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char32_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u32char"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char> value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '"');
			Sink::write(handle, value);
			Sink::write(handle, '"');
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<wchar_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write wstring"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char8_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u8string"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char16_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u16string"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char32_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u32string"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::serialization_types::unit_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write unit type"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::nullptr_t value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "null");
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::nullopt_t value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "null");
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serialization_types::boolean Bool>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			Bool value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, value ? "true" : "false");
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::byte value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: avoid allocations"
			Sink::write(handle, std::to_string(static_cast<std::uint8_t>(value)));
			return false;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::serialization_types::integral I>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			I value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: avoid allocations"
			if constexpr(not std::same_as<__uint128_t, I> && not std::same_as<__int128_t, I>)
				Sink::write(handle, std::to_string(value));
			return false;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::serialization_types::floating_point F>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			F value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: avoid allocations"
			Sink::write(handle, std::to_string(value));
			return false;
		}

		template<roy::serialization::serialization_sink Sink, typename Rep, typename Period>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::chrono::duration<Rep, Period> value
		) -> std::expected<result_type, error_type>{
			#warning write duration
			return false;
		}

		template<roy::serialization::serialization_sink Sink, typename Clock, typename Duration>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::chrono::time_point<Clock, Duration> value
		) -> std::expected<result_type, error_type>{
			#warning write time_point
			return false;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ObjectStart>
			requires(ObjectStart == roy::serialization::structural_type::object_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '{');
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ObjectEnd>
			requires(ObjectEnd == roy::serialization::structural_type::object_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '}');
			return true;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type PropStart>
			requires(PropStart == roy::serialization::structural_type::property_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count,
			roy::serialization::detail::data_type property_type,
			std::string_view property_name,
			std::size_t property_index
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '"');
			Sink::write(handle, property_name);
			Sink::write(handle, '"');
			Sink::write(handle, ':');
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type PropEnd>
			requires(PropEnd == roy::serialization::structural_type::property_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count,
			roy::serialization::detail::data_type property_type,
			std::string_view property_name,
			std::size_t property_index
		) -> std::expected<result_type, error_type>{
			if((property_index + 1) < property_count)
				Sink::write(handle, ',');
			return true;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayStart>
			requires(ArrayStart == roy::serialization::structural_type::array_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '[');
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayEnd>
			requires(ArrayEnd == roy::serialization::structural_type::array_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, ']');
			return true;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayValueStart>
			requires(ArrayValueStart == roy::serialization::structural_type::array_value_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count,
			std::size_t value_index
		) -> std::expected<result_type, error_type>{
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayValueEnd>
			requires(ArrayValueEnd == roy::serialization::structural_type::array_value_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count,
			std::size_t value_index
		) -> std::expected<result_type, error_type>{
			if((value_index + 1) < value_count)
				Sink::write(handle, ',');
			return true;
		}
	};

	struct xml_settings{};

	class xml_serializer{
	public:
		using char_type = char;
		using settings_type = roy::serialization::xml_settings;
		struct state_type{};
		using result_type = bool;
		using error_type = std::string;
		using creation_error_type = std::string;

		template<roy::serialization::serialization_sink Sink>
		using templated_result_type = result_type;

		template<roy::serialization::serialization_sink Sink>
		using templated_error_type = error_type;

		static constexpr auto create(settings_type&& settings)
			-> std::expected<state_type, creation_error_type>{
			return state_type{};
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, value);
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			wchar_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write wchar"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char8_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u8char"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char16_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u16char"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			char32_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u32char"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char> value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, value);
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<wchar_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write wstring"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char8_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u8string"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char16_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u16string"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::basic_string_view<char32_t> value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write u32string"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::serialization_types::unit_t value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: write unit type"
			return false;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::nullptr_t value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "null");
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::nullopt_t value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "null");
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serialization_types::boolean Bool>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			Bool value
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, value ? "true" : "false");
			return true;
		}
		template<roy::serialization::serialization_sink Sink>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::byte value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: avoid allocations"
			Sink::write(handle, std::to_string(static_cast<std::uint8_t>(value)));
			return false;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::serialization_types::integral I>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			I value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: avoid allocations"
			if constexpr(not std::same_as<__uint128_t, I> && not std::same_as<__int128_t, I>)
				Sink::write(handle, std::to_string(value));
			return false;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::serialization_types::floating_point F>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			F value
		) -> std::expected<result_type, error_type>{
			#warning "TODO: avoid allocations"
			Sink::write(handle, std::to_string(value));
			return false;
		}

		template<roy::serialization::serialization_sink Sink, typename Rep, typename Period>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::chrono::duration<Rep, Period> value
		) -> std::expected<result_type, error_type>{
			#warning write duration
			return false;
		}

		template<roy::serialization::serialization_sink Sink, typename Clock, typename Duration>
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::chrono::time_point<Clock, Duration> value
		) -> std::expected<result_type, error_type>{
			#warning write time_point
			return false;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ObjectStart>
			requires(ObjectStart == roy::serialization::structural_type::object_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '<');
			Sink::write(handle, type_name);
			Sink::write(handle, '>');
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ObjectEnd>
			requires(ObjectEnd == roy::serialization::structural_type::object_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '<');
			Sink::write(handle, '/');
			Sink::write(handle, type_name);
			Sink::write(handle, '>');
			return true;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type PropStart>
			requires(PropStart == roy::serialization::structural_type::property_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count,
			roy::serialization::detail::data_type property_type,
			std::string_view property_name,
			std::size_t property_index
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '<');
			Sink::write(handle, property_name);
			Sink::write(handle, '>');
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type PropEnd>
			requires(PropEnd == roy::serialization::structural_type::property_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			std::string_view type_name,
			std::size_t property_count,
			roy::serialization::detail::data_type property_type,
			std::string_view property_name,
			std::size_t property_index
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, '<');
			Sink::write(handle, '/');
			Sink::write(handle, property_name);
			Sink::write(handle, '>');
			return true;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayStart>
			requires(ArrayStart == roy::serialization::structural_type::array_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "<Array>");
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayEnd>
			requires(ArrayEnd == roy::serialization::structural_type::array_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "</Array>");
			return true;
		}

		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayValueStart>
			requires(ArrayValueStart == roy::serialization::structural_type::array_value_start)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count,
			std::size_t value_index
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "<Value>");
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::structural_type ArrayValueEnd>
			requires(ArrayValueEnd == roy::serialization::structural_type::array_value_end)
		static constexpr auto serialize(
			state_type& state,
			Sink::handle_type& handle,
			roy::serialization::detail::data_type values_type,
			std::size_t value_count,
			std::size_t value_index
		) -> std::expected<result_type, error_type>{
			Sink::write(handle, "</Value>");
			return true;
		}
	};

	static_assert(roy::serialization::serialization_sink<size_counter_sink>);
	static_assert(roy::serialization::serialization_sink<stringstream_sink>);
	static_assert(roy::serialization::serialization_sink<file_sink>);
	static_assert(roy::serialization::serialization_sink<binary_file_sink>);
	static_assert(roy::serialization::serializer<json_serializer, stringstream_sink>);
	static_assert(roy::serialization::serializer<xml_serializer, stringstream_sink>);
}

namespace roy::serialization::detail{
	class serialization_bridge{
	public:
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer, typename T>
		static constexpr auto serialize(
			const T& data,
			typename Sink::settings_type&& sink_settings,
			typename Serializer::settings_type&& serializer_settings
		) -> typename Sink::result_type{
			std::expected<typename Serializer::state_type, typename Serializer::creation_error_type> serializer_state{
				Serializer::create(std::move(serializer_settings))
			};

			#warning "TODO: return error"
			//if(not serializer.has_value())
				//return serializer.error(); // TODO: transform_error

			std::expected<typename Sink::handle_type, typename Sink::opening_error_type> sink_handle{
				Sink::open(std::move(sink_settings))
			};

			#warning "TODO: return error"
			//if(not sink.has_value())
				//return sink.error(); // TODO: transform_error

			roy::serialization::detail::serialization_read_buffer buffer{
				roy::serialization::detail::binary_serializer::serialize(data).value()
			};
			roy::serialization::detail::serialization_read_buffer_view view{buffer};

			std::optional<roy::serialization::detail::data_info> data_info{view.read_data_info()};
			if(not data_info.has_value())
				return Sink::empty_result(sink_handle.value());
				//return Sink::empty_result(std::move(sink_handle.value()));

			roy::serialization::detail::serialization_bridge bridge{
				buffer.m_strings_capacity > 0 ? std::make_unique<char[]>(buffer.m_strings_capacity) : nullptr,
				buffer.m_strings_capacity,

				buffer.m_wstrings_capacity > 0 ? std::make_unique<wchar_t[]>(buffer.m_wstrings_capacity) : nullptr,
				buffer.m_wstrings_capacity,

				buffer.m_u8strings_capacity > 0 ? std::make_unique<char8_t[]>(buffer.m_u8strings_capacity) : nullptr,
				buffer.m_u8strings_capacity,

				buffer.m_u16strings_capacity > 0 ? std::make_unique<char16_t[]>(buffer.m_u16strings_capacity) : nullptr,
				buffer.m_u16strings_capacity,

				buffer.m_u32strings_capacity > 0 ? std::make_unique<char32_t[]>(buffer.m_u32strings_capacity) : nullptr,
				buffer.m_u32strings_capacity
			};

			bridge.template serialize<Sink, Serializer>(view, data_info.value(), serializer_state.value(), sink_handle.value());

			std::expected<typename Sink::result_type, typename Sink::closing_error_type> result{
				Sink::close(sink_handle.value())
				//Sink::close(std::move(sink_handle.value()))
			};

			#warning "TODO: return error"
			//if(not result.has_value())
				//return result.error(); // TODO: transform_error

			return result.value();
		}
	private:
		constexpr serialization_bridge(
			std::unique_ptr<char[]> strings,
			std::size_t strings_capacity,

			std::unique_ptr<wchar_t[]> wstrings,
			std::size_t wstrings_capacity,

			std::unique_ptr<char8_t[]> u8strings,
			std::size_t u8strings_capacity,

			std::unique_ptr<char16_t[]> u16strings,
			std::size_t u16strings_capacity,

			std::unique_ptr<char32_t[]> u32strings,
			std::size_t u32strings_capacity
		) :
			m_strings{std::move(strings)},
			m_strings_size{0},
			m_strings_capacity{strings_capacity},

			m_wstrings{std::move(wstrings)},
			m_wstrings_size{0},
			m_wstrings_capacity{wstrings_capacity},

			m_u8strings{std::move(u8strings)},
			m_u8strings_size{0},
			m_u8strings_capacity{u8strings_capacity},

			m_u16strings{std::move(u16strings)},
			m_u16strings_size{0},
			m_u16strings_capacity{u16strings_capacity},

			m_u32strings{std::move(u32strings)},
			m_u32strings_size{0},
			m_u32strings_capacity{u32strings_capacity}{

			if(m_strings_capacity > 0)
				std::memset(m_strings.get(), 0, m_strings_capacity);

			if(m_wstrings_capacity > 0)
				std::memset(m_wstrings.get(), 0, m_wstrings_capacity);

			if(m_u8strings_capacity > 0)
				std::memset(m_u8strings.get(), 0, m_u8strings_capacity);

			if(m_u16strings_capacity > 0)
				std::memset(m_u16strings.get(), 0, m_u16strings_capacity);

			if(m_u32strings_capacity > 0)
				std::memset(m_u32strings.get(), 0, m_u32strings_capacity);
		}
	private:
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			switch(data_info.type){
			case roy::serialization::detail::data_type::unit:
				return serialize_unit<Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::null_value:
				return serialize_null_value<Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::nullopt:
				return serialize_nullopt<Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::boolean:
				return serialize_boolean<Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::byte:
				return serialize_byte<Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::uint8:
				return serialize_unsigned<std::uint8_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::uint16:
				return serialize_unsigned<std::uint16_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::uint32:
				return serialize_unsigned<std::uint32_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::uint64:
				return serialize_unsigned<std::uint64_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
#ifdef __SIZEOF_INT128__
			case roy::serialization::detail::data_type::uint128:
				return serialize_unsigned<__uint128_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
#endif
			case roy::serialization::detail::data_type::int8:
				return serialize_signed<std::int8_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::int16:
				return serialize_signed<std::int16_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::int32:
				return serialize_signed<std::int32_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::int64:
				return serialize_signed<std::int64_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
#ifdef __SIZEOF_INT128__
			case roy::serialization::detail::data_type::int128:
				return serialize_signed<__int128_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
#endif
			case roy::serialization::detail::data_type::float32:
				return serialize_floating_point<float, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::float64:
				return serialize_floating_point<double, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::float128:
				return serialize_floating_point<long double, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::character:
				return serialize_char<char, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::wcharacter:
				return serialize_char<wchar_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::u8character:
				return serialize_char<char8_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::u16character:
				return serialize_char<char16_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::u32character:
				return serialize_char<char32_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::string:
				return serialize_string<char, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::wstring:
				return serialize_string<wchar_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::u8string:
				return serialize_string<char8_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::u16string:
				return serialize_string<char16_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::u32string:
				return serialize_string<char32_t, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::unix_timestamp_days:
				return serialize_timestamp<roy::serialization::serialization_types::days_timestamp, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::unix_timestamp_hours:
				return serialize_timestamp<roy::serialization::serialization_types::hours_timestamp, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::unix_timestamp_minutes:
				return serialize_timestamp<roy::serialization::serialization_types::minutes_timestamp, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::unix_timestamp_seconds:
				return serialize_timestamp<roy::serialization::serialization_types::seconds_timestamp, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::unix_timestamp_milliseconds:
				return serialize_timestamp<roy::serialization::serialization_types::milliseconds_timestamp, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::unix_timestamp_microseconds:
				return serialize_timestamp<roy::serialization::serialization_types::microseconds_timestamp, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::unix_timestamp_nanoseconds:
				return serialize_timestamp<roy::serialization::serialization_types::nanoseconds_timestamp, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_years:
				return serialize_duration<roy::serialization::serialization_types::duration_years, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_months:
				return serialize_duration<roy::serialization::serialization_types::duration_months, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_weeks:
				return serialize_duration<roy::serialization::serialization_types::duration_weeks, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_days:
				return serialize_duration<roy::serialization::serialization_types::duration_days, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_hours:
				return serialize_duration<roy::serialization::serialization_types::duration_hours, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_minutes:
				return serialize_duration<roy::serialization::serialization_types::duration_minutes, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_seconds:
				return serialize_duration<roy::serialization::serialization_types::duration_seconds, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_milliseconds:
				return serialize_duration<roy::serialization::serialization_types::duration_milliseconds, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_microseconds:
				return serialize_duration<roy::serialization::serialization_types::duration_microseconds, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::duration_nanoseconds:
				return serialize_duration<roy::serialization::serialization_types::duration_nanoseconds, Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::array:
				return serialize_array<Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			case roy::serialization::detail::data_type::object:
				return serialize_object<Sink, Serializer>(buffer, data_info, serializer_state, sink_handle);
			default:
				return false;
			}
		}
	private:
		template<typename CharT>
		constexpr auto get_string(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info
		) -> std::optional<std::basic_string_view<CharT>>{
			std::optional<std::size_t> size{roy::serialization::detail::serialization_bridge::get_size(buffer, data_info)};

			if(not size.has_value())
				return std::optional<std::basic_string_view<CharT>>{std::nullopt};

			std::optional<std::span<std::byte>> string_memory{buffer.read_bytes(size.value() * sizeof(CharT))};
			if(not string_memory.has_value() or string_memory.value().size() < (size.value() * sizeof(CharT)))
				return std::optional<std::basic_string_view<CharT>>{std::nullopt};

			if constexpr (std::same_as<CharT, char>){
				std::size_t start_index{m_strings_size};
				m_strings_size += size.value();
				if(m_strings_capacity < (start_index + size.value()))
					return std::optional<std::basic_string_view<CharT>>{std::nullopt};

				if(size.value() == 0)
					return std::basic_string_view<CharT>{m_strings.get(), 0};

				std::memcpy(std::next(m_strings.get(), start_index), string_memory.value().data(), size.value() * sizeof(CharT));
				return std::basic_string_view<CharT>{std::next(m_strings.get(), start_index), size.value()};
			} else if constexpr (std::same_as<CharT, wchar_t>){
				std::size_t start_index{m_wstrings_size};
				m_wstrings_size += size.value();
				if(m_wstrings_capacity < (start_index + size.value()))
					return std::optional<std::basic_string_view<CharT>>{std::nullopt};

				if(size.value() == 0)
					return std::basic_string_view<CharT>{m_wstrings.get(), 0};

				std::memcpy(std::next(m_wstrings.get(), start_index), string_memory.value().data(), size.value() * sizeof(CharT));
				return std::basic_string_view<CharT>{std::next(m_wstrings.get(), start_index), size.value()};
			} else if constexpr (std::same_as<CharT, char8_t>){
				std::size_t start_index{m_u8strings_size};
				m_u8strings_size += size.value();
				if(m_u8strings_capacity < (start_index + size.value()))
					return std::optional<std::basic_string_view<CharT>>{std::nullopt};

				if(size.value() == 0)
					return std::basic_string_view<CharT>{m_u8strings.get(), 0};

				std::memcpy(std::next(m_u8strings.get(), start_index), string_memory.value().data(), size.value() * sizeof(CharT));
				return std::basic_string_view<CharT>{std::next(m_u8strings.get(), start_index), size.value()};
			} else if constexpr (std::same_as<CharT, char16_t>){
				std::size_t start_index{m_u16strings_size};
				m_u16strings_size += size.value();
				if(m_u16strings_capacity < (start_index + size.value()))
					return std::optional<std::basic_string_view<CharT>>{std::nullopt};

				if(size.value() == 0)
					return std::basic_string_view<CharT>{m_u16strings.get(), 0};

				std::memcpy(std::next(m_u16strings.get(), start_index), string_memory.value().data(), size.value() * sizeof(CharT));
				return std::basic_string_view<CharT>{std::next(m_u16strings.get(), start_index), size.value()};
			} else if constexpr (std::same_as<CharT, char32_t>){
				std::size_t start_index{m_u32strings_size};
				m_u32strings_size += size.value();
				if(m_u32strings_capacity < (start_index + size.value()))
					return std::optional<std::basic_string_view<CharT>>{std::nullopt};

				if(size.value() == 0)
					return std::basic_string_view<CharT>{m_u32strings.get(), 0};

				std::memcpy(std::next(m_u32strings.get(), start_index), string_memory.value().data(), size.value() * sizeof(CharT));
				return std::basic_string_view<CharT>{std::next(m_u32strings.get(), start_index), size.value()};
			}

			return std::optional<std::basic_string_view<CharT>>{std::nullopt};
		}

		static constexpr auto get_size(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info
		) -> std::optional<std::size_t>{
			switch(data_info.size_bytes){
			case roy::serialization::detail::size_bytes::one_byte:
				return roy::serialization::detail::serialization_bridge::read_value<std::uint8_t>(buffer, data_info)
					.transform([](std::uint8_t value){ return static_cast<std::size_t>(value); });
			case roy::serialization::detail::size_bytes::two_bytes:
				return roy::serialization::detail::serialization_bridge::read_value<std::uint16_t>(buffer, data_info)
					.transform([](std::uint16_t value){ return static_cast<std::size_t>(value); });
			case roy::serialization::detail::size_bytes::three_bytes:
				return roy::serialization::detail::serialization_bridge::read_value<std::uint32_t>(buffer, data_info)
					.transform([](std::uint32_t value){ return static_cast<std::size_t>(value); });
			case roy::serialization::detail::size_bytes::four_bytes: [[fallthrough]];
			default:
				return roy::serialization::detail::serialization_bridge::read_value<std::uint64_t>(buffer, data_info)
					.transform([](std::uint64_t value){ return static_cast<std::size_t>(value); });
			}
		}

		template<typename T>
		static constexpr auto read_value(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info /*data_info*/
		) -> std::optional<T>{
			std::optional<std::span<std::byte>> bytes{buffer.read_bytes(sizeof(T))};
			if(not bytes.has_value() or bytes.value().size() < sizeof(T))
				return std::optional<T>{std::nullopt};

			alignas(T) std::array<std::byte, sizeof(T)> result_data;
			std::memcpy(result_data.data(), bytes.value().data(), sizeof(T));
			return std::optional<T>{std::in_place, std::bit_cast<T>(result_data)};
		}
	private:
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_unit(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			#warning "TODO: serialize unit type"
			return false;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_null_value(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			Serializer::template serialize<Sink>(serializer_state, sink_handle, nullptr);
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_nullopt(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			Serializer::template serialize<Sink>(serializer_state, sink_handle, std::nullopt);
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_boolean(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<bool> value{roy::serialization::detail::serialization_bridge::read_value<bool>(buffer, data_info)};

			if(not value.has_value())
				return false;

			Serializer::template serialize<Sink>(serializer_state, sink_handle, value.value());
			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_byte(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<std::span<std::byte>> bytes{buffer.read_bytes(1)};

			if(not bytes.has_value() or bytes.value().size() < 1)
				return false;

			Serializer::template serialize<Sink>(serializer_state, sink_handle, bytes.value()[0]);
			return true;
		}
		template<typename Unsigned, roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_unsigned(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<Unsigned> value{roy::serialization::detail::serialization_bridge::read_value<Unsigned>(buffer, data_info)};

			if(not value.has_value())
				return false;

			Serializer::template serialize<Sink>(serializer_state, sink_handle, value.value());
			return true;
		}
		template<typename Signed, roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_signed(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<Signed> value{roy::serialization::detail::serialization_bridge::read_value<Signed>(buffer, data_info)};

			if(not value.has_value())
				return false;

			Serializer::template serialize<Sink>(serializer_state, sink_handle, value.value());
			return true;
		}
		template<typename Float, roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_floating_point(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<Float> value{roy::serialization::detail::serialization_bridge::read_value<Float>(buffer, data_info)};

			if(not value.has_value())
				return false;

			Serializer::template serialize<Sink>(serializer_state, sink_handle, value.value());
			return true;
		}
		template<typename CharT, roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_char(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<CharT> value{roy::serialization::detail::serialization_bridge::read_value<CharT>(buffer, data_info)};

			if(not value.has_value())
				return false;

			Serializer::template serialize<Sink>(serializer_state, sink_handle, value.value());
			return true;
		}
		template<typename CharT, roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_string(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<std::basic_string_view<CharT>> value{get_string<CharT>(buffer, data_info)};

			if(not value.has_value())
				return false;

			Serializer::template serialize<Sink>(serializer_state, sink_handle, value.value());
			return true;
		}
		template<typename Timestamp, roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_timestamp(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			#warning "TODO: serialize timestamp"
			return false;
		}
		template<typename Duration, roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_duration(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			#warning "TODO: serialize duration"
			return false;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_array(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<std::size_t> element_count{roy::serialization::detail::serialization_bridge::get_size(buffer, data_info)};
			if(not element_count.has_value())
				return false;

			std::optional<roy::serialization::detail::data_info> element_data_info{buffer.read_data_info()};
			if(not element_data_info.has_value())
				return false;

			Serializer::template serialize<Sink, roy::serialization::structural_type::array_start>(
				serializer_state,
				sink_handle,
				element_data_info.value().type,
				element_count.value()
			);

			for(std::size_t i{0}; i < element_count.value(); ++i){
				Serializer::template serialize<Sink, roy::serialization::structural_type::array_value_start>(
					serializer_state,
					sink_handle,
					element_data_info.value().type,
					element_count.value(),
					i
				);

				if(not serialize<Sink, Serializer>(buffer, element_data_info.value(), serializer_state, sink_handle))
					return false;

				Serializer::template serialize<Sink, roy::serialization::structural_type::array_value_end>(
					serializer_state,
					sink_handle,
					element_data_info.value().type,
					element_count.value(),
					i
				);
			}

			Serializer::template serialize<Sink, roy::serialization::structural_type::array_end>(
				serializer_state,
				sink_handle,
				element_data_info.value().type,
				element_count.value()
			);

			return true;
		}
		template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer>
		constexpr auto serialize_object(
			roy::serialization::detail::serialization_read_buffer_view& buffer,
			roy::serialization::detail::data_info data_info,
			typename Serializer::state_type& serializer_state,
			typename Sink::handle_type& sink_handle
		) -> bool{
			std::optional<std::basic_string_view<char>> identifier{get_string<char>(buffer, data_info)};
			if(not identifier.has_value())
				return false;

			std::optional<std::size_t> property_count{roy::serialization::detail::serialization_bridge::get_size(buffer, data_info)};
			if(not property_count.has_value())
				return false;

			Serializer::template serialize<Sink, roy::serialization::structural_type::object_start>(
				serializer_state,
				sink_handle,
				identifier.value(),
				property_count.value()
			);

			for(std::size_t i{0}; i < property_count.value(); ++i){
				std::optional<roy::serialization::detail::data_info> property_data_info{buffer.read_data_info()};
				if(not property_data_info.has_value())
					return false;

				std::optional<std::basic_string_view<char>> property_name{get_string<char>(buffer, property_data_info.value())};
				if(not property_name.has_value())
					return false;

				Serializer::template serialize<Sink, roy::serialization::structural_type::property_start>(
					serializer_state,
					sink_handle,
					identifier.value(),
					property_count.value(),
					property_data_info.value().type,
					property_name.value(),
					i
				);

				if(not serialize<Sink, Serializer>(buffer, property_data_info.value(), serializer_state, sink_handle))
					return false;

				Serializer::template serialize<Sink, roy::serialization::structural_type::property_end>(
					serializer_state,
					sink_handle,
					identifier.value(),
					property_count.value(),
					property_data_info.value().type,
					property_name.value(),
					i
				);
			}

			Serializer::template serialize<Sink, roy::serialization::structural_type::object_end>(
				serializer_state,
				sink_handle,
				identifier.value(),
				property_count.value()
			);

			return true;
		}
	private:
		std::unique_ptr<char[]> m_strings;
		std::size_t m_strings_size;
		std::size_t m_strings_capacity;

		std::unique_ptr<wchar_t[]> m_wstrings;
		std::size_t m_wstrings_size;
		std::size_t m_wstrings_capacity;

		std::unique_ptr<char8_t[]> m_u8strings;
		std::size_t m_u8strings_size;
		std::size_t m_u8strings_capacity;

		std::unique_ptr<char16_t[]> m_u16strings;
		std::size_t m_u16strings_size;
		std::size_t m_u16strings_capacity;

		std::unique_ptr<char32_t[]> m_u32strings;
		std::size_t m_u32strings_size;
		std::size_t m_u32strings_capacity;
	};
}

namespace roy::serialization{
	template<roy::serialization::serialization_sink Sink, roy::serialization::serializer<Sink> Serializer, typename T>
	inline constexpr auto serialize(
		const T& data,
		typename Sink::settings_type&& sink_settings,
		typename Serializer::settings_type&& serializer_settings
	) -> typename Sink::result_type{
		return roy::serialization::detail::serialization_bridge::serialize<Sink, Serializer, T>(
			data,
			std::move(sink_settings),
			std::move(serializer_settings)
		);
	}
}

#endif
