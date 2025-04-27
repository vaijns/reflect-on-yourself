#ifndef H_REFLECT_ON_YOURSELF_EXTENSIONS_DEFAULT_BUILDERS
#define H_REFLECT_ON_YOURSELF_EXTENSIONS_DEFAULT_BUILDERS

#include "../builder/builder.hpp"
#include "./builders.hpp"
#include "../builder/extended_builder.hpp"
#include "../util/inplace_string.hpp"
#include "../util/get_name.hpp"

#include "./name.hpp"
#include "./identifier.hpp"
#include "./fields.hpp"
#include "./annotation.hpp"

namespace roy::extensions{
	struct default_builders_extension_tag{};
}

namespace roy::detail{
	template<typename For>
	struct default_builders;

	template<
		typename Type,
		typename... AvailableBuilderTags,
		typename... ExtensionTags,
		typename... Extensions
	> struct default_builders<
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>{
		using type = roy::util::type_wrapper<
			roy::extensions::name_extension_tag,
			roy::extensions::identifier_extension_tag,
			roy::extensions::fields_extension_tag,
			roy::extensions::annotation_extension_tag
		>;
	};

	template<
		auto FieldPtr,
		typename... AvailableBuilderTags,
		typename... ExtensionTags,
		typename... Extensions
	> struct default_builders<
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>{
		using type = roy::util::type_wrapper<
			roy::extensions::name_extension_tag,
			roy::extensions::identifier_extension_tag,
			roy::extensions::annotation_extension_tag
		>;
	};

	template<typename For>
	using default_builders_t = roy::detail::default_builders<For>::type;
}

template<typename For>
struct roy::detail::builder<roy::extensions::default_builders_extension_tag, For>{
	using with_default_builders = roy::detail::add_builders_pack<For, roy::detail::default_builders_t<For>>::type;
};
#endif
