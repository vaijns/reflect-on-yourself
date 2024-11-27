#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FIELD_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FIELD_POINTER

#include "./is_field_pointer.hpp"

namespace roy::util{
	template<auto Ptr>
	struct type_of_field_pointer;

	template<typename TType, TType* Ptr>
		requires(field_pointer<Ptr>)
	struct type_of_field_pointer<Ptr>{
		using type = TType;
	};

	template<auto Ptr>
	using type_of_field_pointer_t = type_of_field_pointer<Ptr>::type;
}
#endif
