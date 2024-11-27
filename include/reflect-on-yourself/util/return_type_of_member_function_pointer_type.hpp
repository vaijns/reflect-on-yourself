#ifndef H_REFLECT_ON_YOURSELF_UTIL_RETURN_TYPE_OF_MEMBER_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_RETURN_TYPE_OF_MEMBER_FUNCTION_POINTER_TYPE

#include "./is_member_function_pointer_type.hpp"

namespace roy::util{
	template<typename T>
	struct return_type_of_member_function_pointer_type;

	template<typename TType, typename UDeclaringType, typename... VParamTypes>
		requires(member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>)
	struct return_type_of_member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>{
		using type = TType;
	};

	template<typename T>
	using return_type_of_member_function_pointer_type_t = return_type_of_member_function_pointer_type<T>::type;
}
#endif
