#ifndef H_REFLECT_ON_YOURSELF_UTIL_FILTER_MISSING_TAG_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_FILTER_MISSING_TAG_TYPE

#include "./has_tag_type.hpp"
#include "./is_exact_type.hpp"
#include "./is_member_pointer_type.hpp"

namespace roy::util {
	namespace detail {
		template<typename TFilterType, typename UFinishedType, template<typename...> typename VWrapper, typename... VRemainingTypes>
		struct filter_missing_tag_type_impl;

		template<typename TFilterType, typename... UFinishedTypes, template<typename...> typename VWrapper, typename... VRemainingTypes, missing_tag_type<TFilterType> WCurrentType>
		struct filter_missing_tag_type_impl<TFilterType, VWrapper<UFinishedTypes...>, VWrapper, WCurrentType, VRemainingTypes...>
			: filter_missing_tag_type_impl<TFilterType, VWrapper<UFinishedTypes...>, VWrapper, VRemainingTypes...> {
		};

		template<typename TFilterType, typename... UFinishedTypes, template<typename...> typename VWrapper, typename... VRemainingTypes, tag_type<TFilterType> WCurrentType>
		struct filter_missing_tag_type_impl<TFilterType, VWrapper<UFinishedTypes...>, VWrapper, WCurrentType, VRemainingTypes...>
			: filter_missing_tag_type_impl<TFilterType, VWrapper<UFinishedTypes..., WCurrentType>, VWrapper, VRemainingTypes...> {
		};

		template<typename TFilterType, typename... UFinishedTypes, template<typename...> typename VWrapper>
		struct filter_missing_tag_type_impl<TFilterType, VWrapper<UFinishedTypes...>, VWrapper> {
			using type = VWrapper<UFinishedTypes...>;
		};
	} // namespace detail

	template<typename TFilterType, typename UTypes>
	struct filter_missing_tag_type;

	template<typename TFilterType, template<typename...> typename UWrapper, typename... UTypes>
	struct filter_missing_tag_type<TFilterType, UWrapper<UTypes...>>
		: detail::filter_missing_tag_type_impl<TFilterType, UWrapper<>, UWrapper, UTypes...> { };

	template<typename TFilterType, typename UTypes>
	using filter_missing_tag_type_t = filter_missing_tag_type<TFilterType, UTypes>::type;
} // namespace roy::util
#endif
