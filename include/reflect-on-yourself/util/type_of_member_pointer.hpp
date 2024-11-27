#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_MEMBER_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_MEMBER_POINTER

#include "./is_member_pointer.hpp"

namespace roy::util{
	template<auto Ptr>
	struct type_of_member_pointer;

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* Ptr>
		requires(member_pointer<Ptr>)
	struct type_of_member_pointer<Ptr>{
		using type = TType;
	};

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
		requires(member_pointer<Ptr>)
	struct type_of_member_pointer<Ptr>{
		using type = TType;
	};

	template<auto Ptr>
	using type_of_member_pointer_t = type_of_member_pointer<Ptr>::type;
}
#endif
