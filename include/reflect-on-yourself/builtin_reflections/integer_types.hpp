#ifndef H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_INTEGER_TYPES
#define H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_INTEGER_TYPES

#include <cstdint>
#include "../reflection.hpp"
#include "./builtin_type_extension.hpp"

namespace roy::identifier{
	ROY_MAKE_REFLECTION_IDENTIFIER(uint8);
	ROY_MAKE_REFLECTION_IDENTIFIER(uint16);
	ROY_MAKE_REFLECTION_IDENTIFIER(uint32);
	ROY_MAKE_REFLECTION_IDENTIFIER(uint64);
	ROY_MAKE_REFLECTION_IDENTIFIER(uint128);

	ROY_MAKE_REFLECTION_IDENTIFIER(int8);
	ROY_MAKE_REFLECTION_IDENTIFIER(int16);
	ROY_MAKE_REFLECTION_IDENTIFIER(int32);
	ROY_MAKE_REFLECTION_IDENTIFIER(int64);
	ROY_MAKE_REFLECTION_IDENTIFIER(int128);
}

template<> struct roy::provide_reflection<std::uint8_t>
	: roy::reflection::for_type<std::uint8_t>
		::with_name<"uint8">
		::with_identifier<roy::identifier::uint8>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<std::uint16_t>
	: roy::reflection::for_type<std::uint16_t>
		::with_name<"uint16">
		::with_identifier<roy::identifier::uint16>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<std::uint32_t>
	: roy::reflection::for_type<std::uint32_t>
		::with_name<"uint32">
		::with_identifier<roy::identifier::uint32>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<std::uint64_t>
	: roy::reflection::for_type<std::uint64_t>
		::with_name<"uint64">
		::with_identifier<roy::identifier::uint64>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<__uint128_t>
	: roy::reflection::for_type<__uint128_t>
		::with_name<"uint128">
		::with_identifier<roy::identifier::uint128>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

// ---

template<> struct roy::provide_reflection<std::int8_t>
	: roy::reflection::for_type<std::int8_t>
		::with_name<"int8">
		::with_identifier<roy::identifier::int8>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<std::int16_t>
	: roy::reflection::for_type<std::int16_t>
		::with_name<"int16">
		::with_identifier<roy::identifier::int16>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<std::int32_t>
	: roy::reflection::for_type<std::int32_t>
		::with_name<"int32">
		::with_identifier<roy::identifier::int32>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<std::int64_t>
	: roy::reflection::for_type<std::int64_t>
		::with_name<"int64">
		::with_identifier<roy::identifier::int64>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

template<> struct roy::provide_reflection<__int128_t>
	: roy::reflection::for_type<__int128_t>
		::with_name<"int128">
		::with_identifier<roy::identifier::int128>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

#endif
