#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_FIELD_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_IS_FIELD_POINTER

#include <type_traits>

namespace roy::util{
	template<auto Ptr>
	struct is_field_pointer : std::false_type{};

	template<typename TType, typename... VParamTypes, TType (* Ptr)(VParamTypes...)>
	struct is_field_pointer<Ptr> : std::true_type{};

	template<typename TType, TType* Ptr>
	struct is_field_pointer<Ptr> : std::true_type{};

	template<auto Ptr>
	inline constexpr bool is_field_pointer_v = is_field_pointer<Ptr>::value;

	template<auto Ptr>
	concept field_pointer = is_field_pointer_v<Ptr>;
}
#endif
