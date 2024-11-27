#ifndef H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_POINTER

#include "./is_member_pointer.hpp"

namespace roy::util{
	template<auto Ptr>
	struct declaring_type_of_member_pointer;

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* Ptr>
		requires(member_pointer<Ptr>)
	struct declaring_type_of_member_pointer<Ptr>{
		using type = UDeclaringType;
	};

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
		requires(member_pointer<Ptr>)
	struct declaring_type_of_member_pointer<Ptr>{
		using type = UDeclaringType;
	};

	template<auto Ptr>
	using declaring_type_of_member_pointer_t = declaring_type_of_member_pointer<Ptr>::type;

}
#endif
