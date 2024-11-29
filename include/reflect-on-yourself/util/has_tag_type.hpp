#ifndef H_REFLECT_ON_YOURSELF_UTIL_HAS_TAG_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_HAS_TAG_TYPE

#include "./contains_type.hpp"
#include "./has_tags.hpp"

#include <type_traits>

namespace roy::util {
	namespace detail {
		template<typename T, bool HasTagType>
		struct has_tag_type_impl {
			static constexpr bool value = HasTagType;
		};
	} // namespace detail

	template<typename T, typename UTagType>
	struct has_tag_type : detail::has_tag_type_impl<T, has_tags<T> && contains<typename T::tag_list, UTagType>> { };

	template<typename T, typename UTagType>
	inline constexpr bool has_tag_type_v = has_tag_type<T, UTagType>::value;

	template<typename T, typename UTagType>
	concept tag_type = has_tag_type_v<T, UTagType>;

	template<typename T, typename UTagType>
	struct is_missing_tag_type
		: detail::has_tag_type_impl<T, not has_tags<T> || not contains<typename T::tag_list, UTagType>> { };

	template<typename T, typename UTagType>
	inline constexpr bool is_missing_tag_type_v = is_missing_tag_type<T, UTagType>::value;

	template<typename T, typename UTagType>
	concept missing_tag_type = is_missing_tag_type_v<T, UTagType>;
} // namespace roy::util
#endif
