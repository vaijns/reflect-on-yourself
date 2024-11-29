#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FUNCTION_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_OF_FUNCTION_POINTER

#include "./is_function_pointer.hpp"

namespace roy::util {
	template<auto Ptr>
	struct pointer_type_of_function_pointer;

	template<typename TType, typename... VParamTypes, TType (*Ptr)(VParamTypes...)>
		requires(function_pointer<Ptr>)
	struct pointer_type_of_function_pointer<Ptr> {
		using type = TType (*)(VParamTypes...);
	};

	template<auto Ptr>
	using pointer_type_of_function_pointer_t = pointer_type_of_function_pointer<Ptr>::type;
} // namespace roy::util
#endif
