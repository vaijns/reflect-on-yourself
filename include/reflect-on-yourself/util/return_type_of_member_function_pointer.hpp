#ifndef H_REFLECT_ON_YOURSELF_UTIL_RETURN_TYPE_OF_MEMBER_FUNCTION_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_RETURN_TYPE_OF_MEMBER_FUNCTION_POINTER

#include "./is_member_function_pointer.hpp"

namespace roy::util {
	template<auto Ptr>
	struct return_type_of_member_function_pointer;

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
		requires(member_function_pointer<Ptr>)
	struct return_type_of_member_function_pointer<Ptr> {
		using type = TType;
	};

	template<auto Ptr>
	using return_type_of_member_function_pointer_t = return_type_of_member_function_pointer<Ptr>::type;
} // namespace roy::util
#endif
