#ifndef H_REFLECT_ON_YOURSELF_BUILDER_TYPE_BUILDER
#define H_REFLECT_ON_YOURSELF_BUILDER_TYPE_BUILDER

#include "./builder.hpp"
#include "../util/type_wrapper.hpp"

namespace roy::detail{
	template<typename Builder, typename NewExtensionTag, typename NewExtension>
	struct extended_builder;

	template<
		typename Type,
		typename AvailableBuilderTags = roy::util::type_wrapper<>,
		typename ExtensionTags = roy::util::type_wrapper<>,
		typename Extensions = roy::util::type_wrapper<>
	> struct type_builder;

	template<
		typename Type,
		typename... AvailableBuilderTags,
		typename... ExtensionTags,
		typename... Extensions
	> struct type_builder<
		Type,
		roy::util::type_wrapper<AvailableBuilderTags...>,
		roy::util::type_wrapper<ExtensionTags...>,
		roy::util::type_wrapper<Extensions...>
	> : roy::detail::builder<
		AvailableBuilderTags,
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>...{
		struct result_t : Extensions...{
			using type = Type;
		};
		struct result{
			using type = result_t;
		};
		template<typename NewExtensionTag, typename NewExtension>
		using extend = roy::detail::extended_builder<
			roy::detail::type_builder<
				Type,
				roy::util::type_wrapper<AvailableBuilderTags...>,
				roy::util::type_wrapper<ExtensionTags...>,
				roy::util::type_wrapper<Extensions...>
			>,
			NewExtensionTag,
			NewExtension
		>::type;
	};
}

#endif
