#ifndef H_REFLECT_ON_YOURSELF_UTIL_REPLACE_NTH_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_REPLACE_NTH_TYPE

#include <cstddef>

namespace roy::util{
	namespace detail{
		template<std::size_t SearchedIndex, typename TType, typename UReplaceWith, typename VFinishedType, std::size_t CurrentIndex = 0>
		struct replace_nth_type_impl;

		// Empty
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper,
			typename UReplaceWith,
			std::size_t CurrentIndex
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<>,
			UReplaceWith,
			TWrapper<>,
			CurrentIndex
		>{
			using type = TWrapper<>;
		};

		// FinishedTypes > 0
		// RemainingTypes > 0
		// CurrentIndex != SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper, typename... TRemainingTypes,
			typename UReplaceWith,
			typename VCurrentType,
			typename... WFinishedTypes,
			std::size_t CurrentIndex
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType, TRemainingTypes...>,
			UReplaceWith,
			TWrapper<WFinishedTypes...>,
			CurrentIndex
		> : replace_nth_type_impl<
			SearchedIndex,
			TWrapper<TRemainingTypes...>,
			UReplaceWith,
			TWrapper<WFinishedTypes..., VCurrentType>,
			CurrentIndex + 1
		>{
		};

		// FinishedTypes > 0
		// RemainingTypes == 0
		// CurrentIndex != SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper,
			typename UReplaceWith,
			typename VCurrentType,
			typename... WFinishedTypes,
			std::size_t CurrentIndex
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType>,
			UReplaceWith,
			TWrapper<WFinishedTypes...>,
			CurrentIndex
		>{
			using type = TWrapper<WFinishedTypes..., VCurrentType>;
		};

		// FinishedTypes == 0
		// RemainingTypes > 0
		// CurrentIndex != SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper, typename... TRemainingTypes,
			typename UReplaceWith,
			typename VCurrentType,
			std::size_t CurrentIndex
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType, TRemainingTypes...>,
			UReplaceWith,
			TWrapper<>,
			CurrentIndex
		> : replace_nth_type_impl<
			SearchedIndex,
			TWrapper<TRemainingTypes...>,
			UReplaceWith,
			TWrapper<VCurrentType>,
			CurrentIndex + 1
		>{
		};

		// FinishedTypes == 0
		// RemainingTypes == 0
		// CurrentIndex != SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper,
			typename UReplaceWith,
			typename VCurrentType,
			std::size_t CurrentIndex
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType>,
			UReplaceWith,
			TWrapper<>,
			CurrentIndex
		>{
			using type = TWrapper<VCurrentType>;
		};

		// FinishedTypes > 0
		// RemainingTypes > 0
		// CurrentIndex == SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper, typename... TRemainingTypes,
			typename UReplaceWith,
			typename VCurrentType,
			typename... WFinishedTypes
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType, TRemainingTypes...>,
			UReplaceWith,
			TWrapper<WFinishedTypes...>,
			SearchedIndex
		> : replace_nth_type_impl<
			SearchedIndex,
			TWrapper<TRemainingTypes...>,
			UReplaceWith,
			TWrapper<WFinishedTypes..., UReplaceWith>,
			SearchedIndex + 1
		>{
		};

		// FinishedTypes > 0
		// RemainingTypes == 0
		// CurrentIndex == SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper,
			typename UReplaceWith,
			typename VCurrentType,
			typename... WFinishedTypes
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType>,
			UReplaceWith,
			TWrapper<WFinishedTypes...>,
			SearchedIndex
		>{
			using type = TWrapper<WFinishedTypes..., UReplaceWith>;
		};

		// FinishedTypes == 0
		// RemainingTypes > 0
		// CurrentIndex == SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper, typename... TRemainingTypes,
			typename UReplaceWith,
			typename VCurrentType
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType, TRemainingTypes...>,
			UReplaceWith,
			TWrapper<>,
			SearchedIndex
		> : replace_nth_type_impl<
			SearchedIndex,
			TWrapper<TRemainingTypes...>,
			UReplaceWith,
			TWrapper<UReplaceWith>,
			SearchedIndex + 1
		>{
		};

		// FinishedTypes == 0
		// RemainingTypes == 0
		// CurrentIndex == SearchedIndex
		template<
			std::size_t SearchedIndex,
			template<typename...> typename TWrapper,
			typename UReplaceWith,
			typename VCurrentType
		> struct replace_nth_type_impl<
			SearchedIndex,
			TWrapper<VCurrentType>,
			UReplaceWith,
			TWrapper<>,
			SearchedIndex
		>{
			using type = TWrapper<UReplaceWith>;
		};
	}

	template<std::size_t Index, typename TType, typename UReplaceWith>
	struct replace_nth_type;

	template<std::size_t Index, template<typename...> typename TWrapper, typename... TTypes, typename UReplaceWith>
	struct replace_nth_type<Index, TWrapper<TTypes...>, UReplaceWith>
		: detail::replace_nth_type_impl<Index, TWrapper<TTypes...>, UReplaceWith, TWrapper<>>{};

	template<std::size_t Index, typename TType, typename UReplaceWith>
	using replace_nth_type_t = replace_nth_type<Index, TType, UReplaceWith>::type;
}
#endif
