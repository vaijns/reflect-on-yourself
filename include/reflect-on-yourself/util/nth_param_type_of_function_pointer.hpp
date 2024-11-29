#ifndef H_REFLECT_ON_YOURSELF_UTIL_NTH_PARAM_TYPE_OF_FUNCTION_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_NTH_PARAM_TYPE_OF_FUNCTION_POINTER

#include "./is_function_pointer.hpp"
#include "./type_wrapper.hpp"
#include "./nth_type_of.hpp"

namespace roy::util{
	template<std::size_t Index, auto Ptr>
	struct nth_param_type_of_function_pointer;

	template<std::size_t Index, typename TType, typename... VParamTypes, TType (* Ptr)(VParamTypes...)>
		requires(function_pointer<Ptr>)
	struct nth_param_type_of_function_pointer<Index, Ptr>{
		using type = nth_type_of_t<Index, util::type_wrapper<VParamTypes...>>;
	};

	template<std::size_t Index, auto Ptr>
	using nth_param_type_of_function_pointer_t = nth_param_type_of_function_pointer<Index, Ptr>::type;
}
#endif
