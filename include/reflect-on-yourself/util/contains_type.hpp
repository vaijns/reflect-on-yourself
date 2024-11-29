#ifndef H_REFLECT_ON_YOURSELF_UTIL_CONTAINS_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_CONTAINS_TYPE

#include <type_traits>

namespace roy::util{
	template<typename T, typename UContains>
	struct contains_type : std::false_type{};

	template<typename TContains, template<typename...> typename UWrapper, typename UCurrent, typename... Vs>
	struct contains_type<UWrapper<UCurrent, Vs...>, TContains>
		: contains_type<UWrapper<Vs...>, TContains>{};

	template<typename TContains, template<typename...> typename UWrapper, typename... Vs>
	struct contains_type<UWrapper<TContains, Vs...>, TContains>
		: std::true_type{};

	template<typename T, typename UContains>
	inline constexpr bool contains_type_v = contains_type<T, UContains>::value;

	template<typename T, typename UContains>
	concept contains = contains_type_v<T, UContains>;
}
#endif
