#ifndef H_REFLECT_ON_YOURSELF_BUILDER_EXTENDED_BUILDER
#define H_REFLECT_ON_YOURSELF_BUILDER_EXTENDED_BUILDER

#include "../util/type_wrapper.hpp"
#include "./type_builder.hpp"
#include "./field_builder.hpp"

namespace roy::detail{
	template<typename Builder, typename NewExtensionTag, typename NewExtension>
	struct extended_builder;

	template<
		typename Type,
		typename... AvailableExtensionTags,
		typename... ExtensionTags,
		typename... Extensions,
		typename NewExtensionTag,
		typename NewExtension
	> struct extended_builder<
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableExtensionTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>,
		NewExtensionTag,
		NewExtension
	>{
		using type = roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableExtensionTags...>, // TODO: Remove NewExtensionTag
			roy::util::type_wrapper<ExtensionTags..., NewExtensionTag>,
			roy::util::type_wrapper<Extensions..., NewExtension>
		>;
	};

	template<
		auto FieldPtr,
		typename... AvailableExtensionTags,
		typename... ExtensionTags,
		typename... Extensions,
		typename NewExtensionTag,
		typename NewExtension
	> struct extended_builder<
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableExtensionTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>,
		NewExtensionTag,
		NewExtension
	>{
		using type = roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableExtensionTags...>, // TODO: Remove NewExtensionTag
			roy::util::type_wrapper<ExtensionTags..., NewExtensionTag>,
			roy::util::type_wrapper<Extensions..., NewExtension>
		>;
	};
}

#endif
