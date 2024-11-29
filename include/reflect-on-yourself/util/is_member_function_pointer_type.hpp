#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER_TYPE

#include <type_traits>

namespace roy::util {
	template<typename T>
	struct is_member_function_pointer_type : std::false_type { };

	template<typename TType, typename UDeclaringType, typename... VParamTypes>
	struct is_member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)> : std::true_type { };

	template<typename T>
	inline constexpr bool is_member_function_pointer_type_v = is_member_function_pointer_type<T>::value;

	template<typename T>
	concept member_function_pointer_type = is_member_function_pointer_type_v<T>;
} // namespace roy::util
#endif
