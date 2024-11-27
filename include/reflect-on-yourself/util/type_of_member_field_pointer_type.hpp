#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_MEMBER_FIELD_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_MEMBER_FIELD_POINTER_TYPE

#include "./is_member_field_pointer_type.hpp"

namespace roy::util{
	template<typename T>
	struct type_of_member_field_pointer_type;

	template<typename TType, typename UDeclaringType>
		requires(member_field_pointer_type<TType UDeclaringType::*>)
	struct type_of_member_field_pointer_type<TType UDeclaringType::*>{
		using type = TType;
	};

	template<typename T>
	using type_of_member_field_pointer_type_t = type_of_member_field_pointer_type<T>::type;
}
#endif
