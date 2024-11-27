#ifndef H_REFLECT_ON_YOURSELF_UTIL_PARAM_COUNT_OF_MEMBER_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_PARAM_COUNT_OF_MEMBER_FUNCTION_POINTER_TYPE

#include <cstddef>
#include "./is_member_function_pointer_type.hpp"

namespace roy::util{
	template<typename T>
	struct param_count_of_member_function_pointer_type;

	template<typename TType, typename UDeclaringType, typename... VParamTypes>
		requires(member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>)
	struct param_count_of_member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>{
		static constexpr std::size_t value = sizeof...(VParamTypes);
	};

	template<typename T>
	inline constexpr std::size_t param_count_of_member_function_pointer_type_v = param_count_of_member_function_pointer_type<T>::value;
}
#endif
