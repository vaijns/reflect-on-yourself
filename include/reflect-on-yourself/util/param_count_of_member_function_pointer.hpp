#ifndef H_REFLECT_ON_YOURSELF_UTIL_PARAM_COUNT_OF_MEMBER_FUNCTION_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_PARAM_COUNT_OF_MEMBER_FUNCTION_POINTER

#include <cstddef>
#include "./is_member_function_pointer.hpp"

namespace roy::util{
	template<auto Ptr>
	struct param_count_of_member_function_pointer;

	template<typename TType, typename UDeclaringType, typename... VParamTypes, TType (UDeclaringType::* Ptr)(VParamTypes...)>
		requires(member_function_pointer<Ptr>)
	struct param_count_of_member_function_pointer<Ptr>{
		static constexpr std::size_t value = sizeof...(VParamTypes);
	};

	template<auto Ptr>
	inline constexpr std::size_t param_count_of_member_function_pointer_v = param_count_of_member_function_pointer<Ptr>::value;
}
#endif
