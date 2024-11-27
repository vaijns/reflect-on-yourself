#ifndef H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_POINTER_TYPE

#include "./is_member_pointer_type.hpp"

namespace roy::util{
	template<typename T>
	struct declaring_type_of_member_pointer_type;

	template<typename TType, typename UDeclaringType>
		requires(member_pointer_type<TType UDeclaringType::*>)
	struct declaring_type_of_member_pointer_type<TType UDeclaringType::*>{
		using type = UDeclaringType;
	};
	
	template<typename TType, typename UDeclaringType, typename... VParamTypes>
		requires(member_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>)
	struct declaring_type_of_member_pointer_type<TType (UDeclaringType::*)(VParamTypes...)>{
		using type = UDeclaringType;
	};

	template<typename T>
	using declaring_type_of_member_pointer_type_t = declaring_type_of_member_pointer_type<T>::type;

}
#endif
