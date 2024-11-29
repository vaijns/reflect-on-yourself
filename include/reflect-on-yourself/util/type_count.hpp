#ifndef H_REFLECT_ON_YOURSELF_UTIL_TYPE_COUNT
#define H_REFLECT_ON_YOURSELF_UTIL_TYPE_COUNT

#include <cstddef>

namespace roy::util {
	namespace detail {
		template<typename T, std::size_t CurrentIndex = 0>
		struct type_count_impl;

		template<template<typename...> typename TWrapper, typename TCurrentType, typename... URemainingTypes, std::size_t CurrentIndex>
		struct type_count_impl<TWrapper<TCurrentType, URemainingTypes...>, CurrentIndex>
			: type_count_impl<TWrapper<URemainingTypes...>, CurrentIndex + 1> { };

		template<template<typename...> typename TWrapper, std::size_t CurrentIndex>
		struct type_count_impl<TWrapper<>, CurrentIndex> {
			static constexpr std::size_t value = CurrentIndex + 1;
		};
	} // namespace detail

	template<typename T>
	struct type_count : detail::type_count_impl<T> { };

	template<typename T>
	inline constexpr std::size_t type_count_v = type_count<T>::value;
} // namespace roy::util
#endif
