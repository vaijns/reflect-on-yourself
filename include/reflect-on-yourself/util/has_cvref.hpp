#ifndef H_REFLECT_ON_YOURSELF_UTIL_HAS_CVREF
#define H_REFLECT_ON_YOURSELF_UTIL_HAS_CVREF

#include <type_traits>

namespace roy::util {
	template<typename T>
	struct has_cvref : std::false_type { };

	template<typename T>
	struct has_cvref<const T> : std::true_type { };

	template<typename T>
	struct has_cvref<volatile T> : std::true_type { };

	template<typename T>
	struct has_cvref<const volatile T> : std::true_type { };

	template<typename T>
	struct has_cvref<T&> : std::true_type { };

	template<typename T>
	struct has_cvref<const T&> : std::true_type { };

	template<typename T>
	struct has_cvref<volatile T&> : std::true_type { };

	template<typename T>
	struct has_cvref<const volatile T&> : std::true_type { };

	template<typename T>
	struct has_cvref<T&&> : std::true_type { };

	template<typename T>
	struct has_cvref<const T&&> : std::true_type { };

	template<typename T>
	struct has_cvref<volatile T&&> : std::true_type { };

	template<typename T>
	struct has_cvref<const volatile T&&> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<T[N]> : std::false_type { };

	template<typename T, std::size_t N>
	struct has_cvref<const T[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<volatile T[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<const volatile T[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<T (&)[N]> : std::false_type { };

	template<typename T, std::size_t N>
	struct has_cvref<const T (&)[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<volatile T (&)[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<const volatile T (&)[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<T (&&)[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<const T (&&)[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<volatile T (&&)[N]> : std::true_type { };

	template<typename T, std::size_t N>
	struct has_cvref<const volatile T (&&)[N]> : std::true_type { };

	template<typename T>
	inline constexpr bool has_cvref_v = has_cvref<T>::value;

	template<typename T>
	concept cvref = has_cvref_v<T>;
} // namespace roy::util
#endif
