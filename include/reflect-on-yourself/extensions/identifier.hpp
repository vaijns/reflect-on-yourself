#ifndef H_REFLECT_ON_YOURSELF_EXTENSIONS_IDENTIFIER
#define H_REFLECT_ON_YOURSELF_EXTENSIONS_IDENTIFIER

#include "../builder/builder.hpp"
#include "../builder/extended_builder.hpp"

#define ROY_MAKE_REFLECTION_IDENTIFIER(NAME) \
	template<typename T> struct NAME{\
		struct type{ using NAME = T; };\
		struct static_constexpr_field{ static constexpr T NAME{}; };\
		struct static_field{ static T NAME{}; };\
		template<T Value> struct static_constexpr_field_value{ static constexpr T NAME{Value}; };\
		template<T Value> struct static_field_value{ static T NAME{Value}; };\
		struct field{ T NAME{}; };\
		template<T Value> struct field_value{ T NAME{Value}; };\
	};

namespace roy::extensions{
	struct identifier_extension_tag{};
}

template<typename For>
struct roy::detail::builder<roy::extensions::identifier_extension_tag, For>{
	template<template<typename> typename Identifier>
	struct extension{
		template<typename T>
		using identifier = Identifier<T>;
	};

	template<template<typename> typename Identifier>
	using with_identifier = roy::detail::extended_builder<
		For,
		roy::extensions::identifier_extension_tag,
		extension<Identifier>
	>::type;
};
#endif
