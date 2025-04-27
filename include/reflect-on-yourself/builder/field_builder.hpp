#ifndef H_REFLECT_ON_YOURSELF_BUILDER_FIELD_BUILDER
#define H_REFLECT_ON_YOURSELF_BUILDER_FIELD_BUILDER

#include "./builder.hpp"
#include "../util/type_wrapper.hpp"
#include "../util/field_ptr_type.hpp"
#include "../util/field_ptr_declaring_type.hpp"

namespace roy::detail{
	template<typename Builder, typename NewExtensionTag, typename NewExtension, bool RemoveBuilder>
	struct extended_builder;

	template<
		auto FieldPtr,
		typename AvailableBuilderTags = roy::util::type_wrapper<>,
		typename ExtensionTags = roy::util::type_wrapper<>,
		typename Extensions = roy::util::type_wrapper<>
	> struct field_builder;

	template<
		auto FieldPtr,
		typename... AvailableBuilderTags,
		typename... ExtensionTags,
		typename... Extensions
	> struct field_builder<
		FieldPtr,
		roy::util::type_wrapper<AvailableBuilderTags...>,
		roy::util::type_wrapper<ExtensionTags...>,
		roy::util::type_wrapper<Extensions...>
	> : roy::detail::builder<
		AvailableBuilderTags,
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>...{
		struct result_t : Extensions...{
			using declaring_type = roy::util::field_ptr_declaring_type_t<FieldPtr>;
			using field_type = roy::util::field_ptr_type_t<FieldPtr>;
			using field_ptr_type = decltype(FieldPtr);
			static constexpr auto field_ptr() noexcept -> decltype(FieldPtr){
				return FieldPtr;
			}
		};
		struct result{
			using type = result_t;
		};
		template<typename NewExtensionTag, typename NewExtension, bool RemoveBuilder>
		using extend = roy::detail::extended_builder<
			roy::detail::field_builder<
				FieldPtr,
				roy::util::type_wrapper<AvailableBuilderTags...>,
				roy::util::type_wrapper<ExtensionTags...>,
				roy::util::type_wrapper<Extensions...>
			>,
			NewExtensionTag,
			NewExtension,
			RemoveBuilder
		>::type;
	};
}

#endif
