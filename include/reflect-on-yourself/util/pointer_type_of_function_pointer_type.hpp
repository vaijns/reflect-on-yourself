#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FUNCTION_POINTER_TYPE

#include "./is_function_pointer_type.hpp"

namespace roy::util{
	template<typename T>
	struct pointer_type_of_function_pointer_type;

	template<typename TType, typename... VParamTypes>
		requires(function_pointer_type<TType (*)(VParamTypes...)>)
	struct pointer_type_of_function_pointer_type<TType (*)(VParamTypes...)>{
		using type = TType (*)(VParamTypes...);
	};

	template<typename T>
	using pointer_type_of_function_pointer_type_t = pointer_type_of_function_pointer_type<T>::type;
}
#endif
