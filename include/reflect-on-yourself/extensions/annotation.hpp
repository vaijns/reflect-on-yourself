#ifndef H_REFLECT_ON_YOURSELF_EXTENSIONS_ANNOTATION
#define H_REFLECT_ON_YOURSELF_EXTENSIONS_ANNOTATION

#include "../builder/builder.hpp"
#include "../builder/extended_builder.hpp"
#include "../util/inplace_string.hpp"
#include "../util/get_name.hpp"

#include <tuple>
#include <type_traits>

namespace roy::extensions{
	struct annotation_extension_tag{};
}

namespace roy::detail{
	template<typename T>
	concept is_annotation = requires(T t){
		T::annotation_values;
	};

	// ---

	template<typename ForBuilder>
	struct has_annotations : std::false_type{};

	template<typename Type, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions>
	struct has_annotations<
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	> : std::bool_constant<(roy::detail::is_annotation<Extensions> || ...)>{};

	template<auto FieldPtr, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions>
	struct has_annotations<
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	> : std::bool_constant<(roy::detail::is_annotation<Extensions> || ...)>{};

	template<typename ForBuilder>
	inline constexpr bool has_annotations_v{roy::detail::has_annotations<ForBuilder>::value};

	// ---

	template<typename RemainingExtensions, typename FinishedExtensions = roy::util::type_wrapper<>>
	struct filter_annotations;

	template<typename CurrentExtension, typename... RemainingExtensions, typename... FinishedExtensions>
	struct filter_annotations<
		roy::util::type_wrapper<CurrentExtension, RemainingExtensions...>,
		roy::util::type_wrapper<FinishedExtensions...>
	> : std::conditional_t<
		roy::detail::is_annotation<CurrentExtension>,
		filter_annotations<
			roy::util::type_wrapper<RemainingExtensions...>,
			roy::util::type_wrapper<FinishedExtensions...>
		>,
		filter_annotations<
			roy::util::type_wrapper<RemainingExtensions...>,
			roy::util::type_wrapper<FinishedExtensions..., CurrentExtension>
		>
	>{};

	template<typename... FinishedExtensions>
	struct filter_annotations<
		roy::util::type_wrapper<>,
		roy::util::type_wrapper<FinishedExtensions...>
	>{
		using type = roy::util::type_wrapper<FinishedExtensions...>;
	};

	template<typename... Extensions>
	using filter_annotations_t = roy::detail::filter_annotations<Extensions...>::type;

	// ---

	template<typename ForBuilder>
	struct get_annotations{
		static constexpr std::tuple<> value{};
	};

	template<typename Type, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions>
	struct get_annotations<
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>{
		static constexpr auto value{
			std::tuple_cat(std::conditional_t<roy::detail::is_annotation<Extensions>, std::tuple<Extensions>, std::tuple<>>{}...)
		};
	};

	template<auto FieldPtr, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions>
	struct get_annotations<
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>{
		static constexpr auto value{
			std::tuple_cat(std::conditional_t<roy::detail::is_annotation<Extensions>, std::tuple<Extensions>, std::tuple<>>{}...)
		};
	};

	template<typename ForBuilder>
	inline constexpr auto get_annotations_v{roy::detail::get_annotations<ForBuilder>::value};

	// ---

	template<typename ForBuilder>
	struct remove_annotations;

	template<typename Type, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions>
	struct remove_annotations<
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>{
		using type = roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::detail::filter_annotations_t<roy::util::type_wrapper<Extensions...>>
		>;
	};

	template<auto FieldPtr, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions>
	struct remove_annotations<
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>
	>{
		using type = roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::detail::filter_annotations_t<roy::util::type_wrapper<Extensions...>>
		>;
	};

	template<typename ForBuilder>
	using remove_annotations_t = roy::detail::remove_annotations<ForBuilder>::type;

	// ---

	template<typename ForBuilder, auto Annotation>
	struct add_annotation;

	template<typename Type, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions, auto Annotation>
	struct add_annotation<
		roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>,
		Annotation
	>{
		static constexpr auto annotation_values{
			std::tuple_cat(
				roy::detail::get_annotations_v<roy::detail::type_builder<
					Type,
					roy::util::type_wrapper<AvailableBuilderTags...>,
					roy::util::type_wrapper<ExtensionTags...>,
					roy::util::type_wrapper<Extensions...>
				>>,
				std::make_tuple(Annotation)
			)
		};
	};

	template<auto FieldPtr, typename... AvailableBuilderTags, typename... ExtensionTags, typename... Extensions, auto Annotation>
	struct add_annotation<
		roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<AvailableBuilderTags...>,
			roy::util::type_wrapper<ExtensionTags...>,
			roy::util::type_wrapper<Extensions...>
		>,
		Annotation
	>{
		static constexpr auto annotation_values{
			std::tuple_cat(
				roy::detail::get_annotations_v<roy::detail::field_builder<
					FieldPtr,
					roy::util::type_wrapper<AvailableBuilderTags...>,
					roy::util::type_wrapper<ExtensionTags...>,
					roy::util::type_wrapper<Extensions...>
				>>,
				std::make_tuple(Annotation)
			)
		};

		template<typename AnnotationType>
		static constexpr auto annotation() noexcept
			-> AnnotationType{
			return std::get<AnnotationType>(annotation_values);
		}
	};
}

template<typename For>
struct roy::detail::builder<roy::extensions::annotation_extension_tag, For>{
	template<auto Annotation>
	using with_annotation = roy::detail::extended_builder<
		roy::detail::remove_annotations_t<For>,
		roy::extensions::annotation_extension_tag,
		roy::detail::add_annotation<For, Annotation>
	>::type;
};
#endif
