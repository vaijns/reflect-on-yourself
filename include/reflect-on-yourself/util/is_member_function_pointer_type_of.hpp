#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER_TYPE_OF
#define H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER_TYPE_OF

#include <type_traits>

namespace roy::util {
	template<typename T, typename UDeclaringType>
	struct is_member_function_pointer_type_of : std::false_type { };

	template<typename TType, typename UDeclaringType, typename... VParamTypes>
	struct is_member_function_pointer_type_of<TType (UDeclaringType::*)(VParamTypes...), UDeclaringType>
		: std::true_type { };

	template<typename T, typename UDeclaringType>
	inline constexpr bool is_member_function_pointer_type_of_v =
		is_member_function_pointer_type_of<T, UDeclaringType>::value;

	template<typename T, typename UDeclaringType>
	concept member_function_pointer_type_of = is_member_function_pointer_type_of_v<T, UDeclaringType>;
} // namespace roy::util
#endif
