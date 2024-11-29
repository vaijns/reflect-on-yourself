#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER_OF
#define H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_FUNCTION_POINTER_OF

#include <type_traits>

namespace roy::util {
	template<auto Ptr, typename TDeclaringType>
	struct is_member_function_pointer_of : std::false_type { };

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
	struct is_member_function_pointer_of<Ptr, UDeclaringType> : std::true_type { };

	template<auto Ptr, typename TDeclaringType>
	inline constexpr bool is_member_function_pointer_of_v = is_member_function_pointer_of<Ptr, TDeclaringType>::value;

	template<auto Ptr, typename TDeclaringType>
	concept member_function_pointer_of = is_member_function_pointer_of_v<Ptr, TDeclaringType>;
} // namespace roy::util
#endif
