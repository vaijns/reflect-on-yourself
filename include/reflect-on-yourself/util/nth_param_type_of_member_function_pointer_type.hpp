#ifndef H_REFLECT_ON_YOURSELF_UTIL_NTH_PARAM_TYPE_OF_MEMBER_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_NTH_PARAM_TYPE_OF_MEMBER_FUNCTION_POINTER_TYPE

#include "./is_member_function_pointer_type.hpp"
#include "./nth_type_of.hpp"

namespace roy::util{
	template<std::size_t Index, typename T>
	struct nth_param_type_of_member_function_pointer_type;

	template<std::size_t Index, typename TType, typename UDeclaringType, typename... VParamTypes>
		requires(member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>)
	struct nth_param_type_of_member_function_pointer_type<Index, TType (UDeclaringType::*)(VParamTypes...)>{
		using type = nth_type_of_t<Index, VParamTypes...>;
	};

	template<std::size_t Index, typename T>
	using nth_param_type_of_member_function_pointer_type_t = nth_param_type_of_member_function_pointer_type<Index, T>::type;
}
#endif
