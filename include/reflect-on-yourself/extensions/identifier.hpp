#ifndef H_REFLECT_ON_YOURSELF_EXTENSIONS_IDENTIFIER
#define H_REFLECT_ON_YOURSELF_EXTENSIONS_IDENTIFIER

#include "../builder/builder.hpp"
#include "../builder/extended_builder.hpp"

#define ROY_MAKE_REFLECTION_IDENTIFIER(NAME) \
	struct NAME{\
		template<typename T> struct type{ using NAME = T; };\
		template<typename T> struct static_constexpr_field{ static constexpr T NAME{}; };\
		template<typename T> struct static_field{ static T NAME{}; };\
		template<auto Value> struct static_constexpr_field_value{ static constexpr auto NAME{Value}; };\
		template<auto Value> struct static_field_value{ static auto NAME{Value}; };\
		template<typename T> struct field{ T NAME{}; };\
		template<auto Value> struct field_value{ auto NAME{Value}; };\
	};

namespace roy::extensions{
	struct identifier_extension_tag{};
}

template<typename For>
struct roy::detail::builder<roy::extensions::identifier_extension_tag, For>{
	template<typename Identifier>
	struct extension{
		using identifier = Identifier;
	};

	template<typename Identifier>
	using with_identifier = roy::detail::extended_builder<
		For,
		roy::extensions::identifier_extension_tag,
		extension<Identifier>,
		true
	>::type;
};
#endif
