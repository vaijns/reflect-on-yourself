#ifndef H_REFLECT_ON_YOURSELF_UTIL_NTH_PARAM_TYPE_OF_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_NTH_PARAM_TYPE_OF_FUNCTION_POINTER_TYPE

#include "./is_function_pointer_type.hpp"
#include "./nth_type_of.hpp"

namespace roy::util{
	template<std::size_t Index, typename T>
	struct nth_param_type_of_function_pointer_type;

	template<std::size_t Index, typename TType, typename... VParamTypes>
		requires(function_pointer_type<TType (*)(VParamTypes...)>)
	struct nth_param_type_of_function_pointer_type<Index, TType (*)(VParamTypes...)>{
		using type = nth_type_of_t<Index, VParamTypes...>;
	};

	template<std::size_t Index, typename T>
	using nth_param_type_of_function_pointer_type_t = nth_param_type_of_function_pointer_type<Index, T>::type;
}
#endif
