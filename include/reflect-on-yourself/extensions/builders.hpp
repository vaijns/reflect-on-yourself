#ifndef H_REFLECT_ON_YOURSELF_EXTENSIONS_BUILDERS
#define H_REFLECT_ON_YOURSELF_EXTENSIONS_BUILDERS

#include "../builder/builder.hpp"
#include "../builder/extended_builder.hpp"
#include "../util/inplace_string.hpp"
#include "../util/get_name.hpp"

namespace roy::extensions{
	struct builders_extension_tag{};
}

namespace roy::detail{
	template<typename For, typename... BuilderTags>
	struct add_builders;

	template<
		typename Type,
		typename... AvailableBuilderTags,
		typename... ExtensionTags,
		typename... Extensions,
		typename... BuilderTags
	> struct add_builders<
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>,
		BuilderTags...
	>{
		using type = roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags..., BuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>;
	};

	template<
		auto FieldPtr,
		typename... AvailableBuilderTags,
		typename... ExtensionTags,
		typename... Extensions,
		typename... BuilderTags
	> struct add_builders<
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>,
		BuilderTags...
	>{
		using type = roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags..., BuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>;
	};

	template<typename For, typename BuilderTagsPack> // single param, not actual pack
	struct add_builders_pack
		: add_builders<For, BuilderTagsPack>{

	};

	template<typename For, typename... BuilderTags>
	struct add_builders_pack<For, roy::util::type_wrapper<BuilderTags...>>
		: add_builders<For, BuilderTags...>{
	};
}

template<typename For>
struct roy::detail::builder<roy::extensions::builders_extension_tag, For>{
	template<typename... BuilderTags>
	using with_builders = std::conditional_t<
		(sizeof...(BuilderTags) == 1),
		typename roy::detail::add_builders_pack<For, BuilderTags...>::type,
		typename roy::detail::add_builders<For, BuilderTags...>::type
	>;
};
#endif
