#ifndef H_REFLECT_ON_YOURSELF_UTIL_POINTER_TYPE_OF_MEMBER_FUNCTION_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_POINTER_TYPE_OF_MEMBER_FUNCTION_POINTER

#include "./is_member_function_pointer.hpp"

namespace roy::util {
	template<auto Ptr>
	struct pointer_type_of_member_function_pointer;

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::*Ptr)(VParamTypes...)>
		requires(member_function_pointer<Ptr>)
	struct pointer_type_of_member_function_pointer<Ptr> {
		using type = TType (*)(VParamTypes...);
	};

	template<auto Ptr>
	using pointer_type_of_member_function_pointer_t = pointer_type_of_member_function_pointer<Ptr>::type;
} // namespace roy::util
#endif
