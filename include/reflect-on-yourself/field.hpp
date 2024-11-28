#ifndef H_REFLECT_ON_YOURSELF_FIELD
#define H_REFLECT_ON_YOURSELF_FIELD

#include <type_traits>
#include "./util.hpp"

namespace roy{
	template<auto FieldPtr, util::basic_string_literal Name, typename... TExtensions>
		requires(util::member_field_pointer<FieldPtr>)
	struct field{
		using type = util::type_of_member_field_pointer_t<FieldPtr>;
		static constexpr auto name = Name;
		static constexpr auto pointer = FieldPtr;

		struct extensions : TExtensions...{};

		template<typename TExtension>
		using extend = field<FieldPtr, Name, TExtensions..., TExtension>;

		template<util::basic_string_literal AliasName>
		using alias = field<FieldPtr, AliasName, TExtensions...>;
	};

	template<typename TField>
	struct is_field_type : std::false_type{};

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* FieldPtr, util::basic_string_literal Name, typename... VExtensions>
	struct is_field_type<field<FieldPtr, Name, VExtensions...>> : std::true_type{};

	template<typename TField>
	inline constexpr bool is_field_type_v = is_field_type<TField>::value;

	template<typename TField>
	concept field_type = is_field_type_v<TField>;

	// -----

	template<typename TField, typename UDeclaringType>
	struct is_field_type_of : std::false_type{};

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* FieldPtr, util::basic_string_literal Name, typename... VExtensions>
	struct is_field_type_of<field<FieldPtr, Name, VExtensions...>, UDeclaringType> : std::true_type{};

	template<typename TField, typename UDeclaringType>
	inline constexpr bool is_field_type_of_v = is_field_type_of<TField, UDeclaringType>::value;

	template<typename TField, typename UDeclaringType>
	concept field_type_of = is_field_type_of_v<TField, UDeclaringType>;
}
#endif
