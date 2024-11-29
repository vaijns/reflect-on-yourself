#ifndef H_REFLECT_ON_YOURSELF_UTIL_POINTER_TYPE_OF_MEMBER_FUNCTION_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_POINTER_TYPE_OF_MEMBER_FUNCTION_POINTER_TYPE

#include "./is_member_function_pointer_type.hpp"

namespace roy::util {
	template<typename T>
	struct pointer_type_of_member_function_pointer_type;

	template<typename TType, typename UDeclaringType, typename... VParamTypes>
		requires(member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>)
	struct pointer_type_of_member_function_pointer_type<TType (UDeclaringType::*)(VParamTypes...)> {
		using type = TType (*)(VParamTypes...);
	};

	template<typename T>
	using pointer_type_of_member_function_pointer_type_t = pointer_type_of_member_function_pointer_type<T>::type;
} // namespace roy::util
#endif
