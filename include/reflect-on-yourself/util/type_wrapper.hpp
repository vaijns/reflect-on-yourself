#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_WRAPPER
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_WRAPPER

#include <tuple>
#include <cstddef>

namespace roy::util{
	template<typename... Ts>
	struct type_wrapper{};
}

namespace roy::detail{
	template<std::size_t N, typename T>
	struct nth_type;

	template<std::size_t N, typename... Ts>
	struct nth_type<N, roy::util::type_wrapper<Ts...>>{
		using type = std::tuple_element_t<N, std::tuple<Ts...>>;
	};
}

namespace roy::util{
	template<std::size_t N, typename T>
	using nth_type = roy::detail::nth_type<N, T>::type;
}

#endif
