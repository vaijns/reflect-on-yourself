#ifndef H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_FIELD_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_FIELD_POINTER

#include "./is_member_field_pointer.hpp"

namespace roy::util{
	template<auto Ptr>
	struct declaring_type_of_member_field_pointer;

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* Ptr>
		requires(member_field_pointer<Ptr>)
	struct declaring_type_of_member_field_pointer<Ptr>{
		using type = UDeclaringType;
	};

	template<auto Ptr>
	using declaring_type_of_member_field_pointer_t = declaring_type_of_member_field_pointer<Ptr>::type;
}
#endif
