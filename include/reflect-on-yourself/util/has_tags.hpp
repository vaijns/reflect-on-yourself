#ifndef H_REFLECT_ON_YOURSELF_UTIL_HAS_TAGS
#define H_REFLECT_ON_YOURSELF_UTIL_HAS_TAGS

#include <type_traits>
#include "./is_exact_type.hpp"

namespace roy::util{
	template<typename T>
	concept has_tags = requires{
		typename T::tag_list;
	};
}
#endif
