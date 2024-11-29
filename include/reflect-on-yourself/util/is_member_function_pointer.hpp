#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER

#include <type_traits>

namespace roy::util {
	template<auto Ptr>
	struct is_member_function_pointer : std::false_type { };

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
	struct is_member_function_pointer<Ptr> : std::true_type { };

	template<auto Ptr>
	inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<Ptr>::value;

	template<auto Ptr>
	concept member_function_pointer = is_member_function_pointer_v<Ptr>;
} // namespace roy::util
#endif
