#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_POINTER
#define H_REFLECT_ON_YOURSELF_UTIL_IS_MEMBER_POINTER

#include <type_traits>

namespace roy::util {
	template<auto Ptr>
	struct is_member_pointer : std::false_type { };

	template<typename TType, typename UDeclaringType, TType UDeclaringType::* Ptr>
	struct is_member_pointer<Ptr> : std::true_type { };

	template<auto Ptr>
	inline constexpr bool is_member_pointer_v = is_member_pointer<Ptr>::value;

	template<auto Ptr>
	concept member_pointer = is_member_pointer_v<Ptr>;
} // namespace roy::util
#endif
