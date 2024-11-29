#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_MEMBER_FIELD_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_MEMBER_FIELD_POINTER

#include "./is_member_field_pointer.hpp"

namespace roy::util {
	template<auto Ptr>
	struct type_of_member_field_pointer;

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* Ptr>
		requires(member_field_pointer<Ptr>)
	struct type_of_member_field_pointer<Ptr> {
		using type = TType;
	};

	template<auto Ptr>
	using type_of_member_field_pointer_t = type_of_member_field_pointer<Ptr>::type;
} // namespace roy::util
#endif
