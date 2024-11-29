#ifndef H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_FUNCTION_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_DECLARING_TYPE_OF_MEMBER_FUNCTION_POINTER

#include "./is_member_function_pointer.hpp"

namespace roy::util {
	template<auto Ptr>
	struct declaring_type_of_member_function_pointer;

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
		requires(member_function_pointer<Ptr>)
	struct declaring_type_of_member_function_pointer<Ptr> {
		using type = UDeclaringType;
	};

	template<auto Ptr>
	using declaring_type_of_member_function_pointer_t = declaring_type_of_member_function_pointer<Ptr>::type;
} // namespace roy::util
#endif
