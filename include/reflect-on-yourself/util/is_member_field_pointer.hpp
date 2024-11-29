#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FIELD_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FIELD_POINTER

#include <type_traits>

namespace roy::util {
	template<auto Ptr>
	struct is_member_field_pointer : std::false_type { };

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
	struct is_member_field_pointer<Ptr> : std::false_type { };

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* Ptr>
	struct is_member_field_pointer<Ptr> : std::true_type { };

	template<auto Ptr>
	inline constexpr bool is_member_field_pointer_v = is_member_field_pointer<Ptr>::value;

	template<auto Ptr>
	concept member_field_pointer = is_member_field_pointer_v<Ptr>;
} // namespace roy::util
#endif
