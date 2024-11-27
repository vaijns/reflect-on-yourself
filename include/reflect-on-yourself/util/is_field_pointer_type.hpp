#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_FIELD_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_IS_FIELD_POINTER_TYPE

#include <type_traits>

namespace roy::util{
	template<typename T>
	struct is_field_pointer_type : std::false_type{};

	template<typename TType, typename... VParamTypes>
	struct is_field_pointer_type<TType (*)(VParamTypes...)> : std::false_type{};

	template<typename TType>
	struct is_field_pointer_type<TType*> : std::true_type{};

	template<typename T>
	inline constexpr bool is_field_pointer_type_v = is_field_pointer_type<T>::value;

	template<typename T>
	concept field_pointer_type = is_field_pointer_type_v<T>;
}
#endif
