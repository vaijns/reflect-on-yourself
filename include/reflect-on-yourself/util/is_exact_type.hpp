#ifndef H_REFLECT_ON_YOURSELF_UTIL_IS_EXACT_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_IS_EXACT_TYPE

#include <type_traits>

namespace roy::util {
	template<typename T, typename U>
	struct is_exact_type : std::false_type { };

	template<typename T>
	struct is_exact_type<T, T> : std::true_type { };

	template<typename T, typename U>
	inline constexpr bool is_exact_type_v = is_exact_type<T, U>::value;

	template<typename T, typename U>
	concept exact_type = is_exact_type_v<T, U>;
} // namespace roy::util
#endif
