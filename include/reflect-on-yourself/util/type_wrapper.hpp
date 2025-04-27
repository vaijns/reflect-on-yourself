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

	template<typename Type, typename TypeList, typename FinishedTypes = roy::util::type_wrapper<>>
	struct remove_type;

	template<typename Type, typename CurrentType, typename... RemainingTypes, typename... FinishedTypes>
	struct remove_type<
		Type,
		roy::util::type_wrapper<CurrentType, RemainingTypes...>,
		roy::util::type_wrapper<FinishedTypes...>
	> : remove_type<
		Type,
		roy::util::type_wrapper<RemainingTypes...>,
		roy::util::type_wrapper<FinishedTypes..., CurrentType>
	>{

	};

	template<typename CurrentType, typename... RemainingTypes, typename... FinishedTypes>
	struct remove_type<
		CurrentType,
		roy::util::type_wrapper<CurrentType, RemainingTypes...>,
		roy::util::type_wrapper<FinishedTypes...>
	> : remove_type<
		CurrentType,
		roy::util::type_wrapper<RemainingTypes...>,
		roy::util::type_wrapper<FinishedTypes...>
	>{

	};

	template<typename Type, typename... FinishedTypes>
	struct remove_type<
		Type,
		roy::util::type_wrapper<>,
		roy::util::type_wrapper<FinishedTypes...>
	>{
		using type = roy::util::type_wrapper<FinishedTypes...>;
	};
}

namespace roy::util{
	template<std::size_t N, typename T>
	using nth_type = roy::detail::nth_type<N, T>::type;

	template<typename Type, typename TypeList>
	using remove_type = roy::detail::remove_type<Type, TypeList>::type;
}

#endif
