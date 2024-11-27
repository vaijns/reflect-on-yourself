#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FIELD_POINTER_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FIELD_POINTER_TYPE

#include "./is_field_pointer_type.hpp"

namespace roy::util{
	template<typename T>
	struct type_of_field_pointer_type;

	template<typename TType>
		requires(field_pointer_type<TType*>)
	struct type_of_field_pointer_type<TType*>{
		using type = TType;
	};

	template<typename T>
	using type_of_field_pointer_type_t = type_of_field_pointer_type<T>::type;
}
#endif
