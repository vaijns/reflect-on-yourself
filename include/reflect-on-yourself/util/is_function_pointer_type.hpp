#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_IS_FUNCTION_POINTER_TYPE

#include <type_traits>

namespace roy::util{
	template<typename T>
	struct is_function_pointer_type : std::false_type{};

	template<typename TType, typename... VParamTypes>
	struct is_function_pointer_type<TType (*)(VParamTypes...)> : std::true_type{};

	template<typename T>
	inline constexpr bool is_function_pointer_type_v = is_function_pointer_type<T>::value;

	template<typename T>
	concept function_pointer_type = is_function_pointer_type_v<T>;
}
#endif
