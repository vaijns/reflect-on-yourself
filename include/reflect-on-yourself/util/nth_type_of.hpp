#ifndef H_REFLECT_ON_YOURSELF_UTIL_NTH_TYPE_OF
#define H_REFLECT_ON_YOURSELF_UTIL_NTH_TYPE_OF

#include <cstddef>

namespace roy::util{
	namespace detail{
		template<std::size_t CurrentIndex, std::size_t SearchedIndex, typename... Ts>
		concept searched_index_in_range = (SearchedIndex < (CurrentIndex + sizeof...(Ts)));

		template<std::size_t CurrentIndex, std::size_t SearchedIndex, typename... Ts>
		struct nth_type_of_impl;

		template<std::size_t Index, typename T, typename... Ts>
		struct nth_type_of_impl<Index, Index, T, Ts...>{
			using type = T;
		};

		template<std::size_t CurrentIndex, std::size_t SearchedIndex, typename T, typename... Ts>
			requires(searched_index_in_range<CurrentIndex, SearchedIndex, T, Ts...>)
		struct nth_type_of_impl<CurrentIndex, SearchedIndex, T, Ts...>
			: nth_type_of_impl<CurrentIndex + 1, SearchedIndex, Ts...>{};
	}

	template<std::size_t Index, typename... Ts>
	struct nth_type_of : detail::nth_type_of_impl<0, Index, Ts...>{};

	template<std::size_t Index, typename... Ts>
	using nth_type_of_t = nth_type_of<Index, Ts...>::type;
}
#endif
