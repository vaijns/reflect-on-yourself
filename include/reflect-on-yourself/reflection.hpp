#ifndef H_REFLECT_ON_YOURSELF_REFLECTION
#define H_REFLECT_ON_YOURSELF_REFLECTION

#include "./builder/type_builder.hpp"
#include "./builder/field_builder.hpp"
#include "./extensions/default_builders.hpp"
#include "reflect-on-yourself/util/field_ptr_declaring_type.hpp"

namespace roy{
	template<typename T>
	struct provide_reflection;

	namespace reflection{
		template<typename Type>
		using for_type = roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<
				roy::extensions::builders_extension_tag,
				roy::extensions::default_builders_extension_tag
			>,
			roy::util::type_wrapper<>,
			roy::util::type_wrapper<>
		>;

		template<auto FieldPtr>
		using for_field = roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<
				roy::extensions::builders_extension_tag,
				roy::extensions::default_builders_extension_tag
			>,
			roy::util::type_wrapper<>,
			roy::util::type_wrapper<>
		>;
	}
}

namespace roy::detail{
	inline constexpr auto field_ptrs_equal(auto field_ptr_a, auto field_ptr_b) noexcept
		-> bool{
		if constexpr(not std::is_same_v<decltype(field_ptr_a), decltype(field_ptr_b)>)
			return false;
		else
			return field_ptr_a == field_ptr_b;
	}

	template<typename SearchedType, typename TypeList>
	struct contains_type_impl;

	template<typename SearchedType, template<typename...> typename Wrapper, typename... Types>
	struct contains_type_impl<SearchedType, Wrapper<Types...>>
		: std::bool_constant<(std::is_same_v<SearchedType, Types> || ...)>{};

	template<typename SearchedType, typename TypeList>
	inline constexpr auto contains_type() noexcept
		-> bool{
		return contains_type_impl<SearchedType, std::remove_cvref_t<TypeList>>::value;
	}

	template<template<typename...> typename SearchedType, typename Type>
	struct is_type_templated_type : std::false_type{};

	template<template<typename...> typename SearchedType, typename... Types>
	struct is_type_templated_type<SearchedType, SearchedType<Types...>> : std::true_type{};

	template<template<typename...> typename SearchedType, typename Type>
	inline constexpr bool is_type_templated_type_v{roy::detail::is_type_templated_type<SearchedType, Type>::value};

	template<template<typename...> typename SearchedType, typename TypeList>
	struct contains_type_templated_type_impl;

	template<template<typename...> typename SearchedType, template<typename...> typename Wrapper, typename... Types>
	struct contains_type_templated_type_impl<SearchedType, Wrapper<Types...>>
		: std::bool_constant<(roy::detail::is_type_templated_type_v<SearchedType, Types> || ...)>{};

	template<template<typename...> typename SearchedType, typename TypeList>
	inline constexpr auto contains_type_templated_type() noexcept
		-> bool{
		return contains_type_templated_type_impl<SearchedType, TypeList>::value;
	}

	template<template<auto...> typename SearchedType, typename Type>
	struct is_value_templated_type : std::false_type{};

	template<template<auto...> typename SearchedType, auto... Values>
	struct is_value_templated_type<SearchedType, SearchedType<Values...>> : std::true_type{};

	template<template<auto...> typename SearchedType, typename Type>
	inline constexpr bool is_value_templated_type_v{roy::detail::is_value_templated_type<SearchedType, Type>::value};

	template<template<auto...> typename SearchedType, typename TypeList>
	struct contains_value_templated_type_impl;

	template<template<auto...> typename SearchedType, template<typename...> typename Wrapper, typename... Types>
	struct contains_value_templated_type_impl<SearchedType, Wrapper<Types...>>
		: std::bool_constant<(roy::detail::is_value_templated_type_v<SearchedType, Types> || ...)>{};

	template<template<auto...> typename SearchedType, typename TypeList>
	inline constexpr auto contains_value_templated_type() noexcept
		-> bool{
		return contains_value_templated_type_impl<SearchedType, TypeList>::value;
	}

	template<template<typename...> typename SearchedType, typename TypeList>
	inline constexpr auto get_type_templated_type_index() noexcept
		-> std::size_t{
		static constexpr std::size_t match_index{
			[]<std::size_t... Is>(std::index_sequence<Is...>){
				static constexpr std::array<bool, sizeof...(Is)> is_equal{
					roy::detail::is_type_templated_type_v<
						SearchedType,
						std::tuple_element_t<Is, TypeList>
					>...
				};

				static_assert((is_equal[Is] || ...), "Templated type not found.");
				for(std::size_t i{0}; i < sizeof...(Is); ++i){
					if(is_equal[i])
						return i;
				}

				std::unreachable();
			}(std::make_index_sequence<std::tuple_size_v<TypeList>>{})
		};

		return match_index;
	}

	template<template<auto...> typename SearchedType, typename TypeList>
	inline constexpr auto get_value_templated_type_index() noexcept
		-> std::size_t{
		static constexpr std::size_t match_index{
			[]<std::size_t... Is>(std::index_sequence<Is...>){
				static constexpr std::array<bool, sizeof...(Is)> is_equal{
					roy::detail::is_value_templated_type_v<
						SearchedType,
						std::tuple_element_t<Is, TypeList>
					>...
				};

				static_assert((is_equal[Is] || ...), "Templated type not found.");
				for(std::size_t i{0}; i < sizeof...(Is); ++i){
					if(is_equal[i])
						return i;
				}

				std::unreachable();
			}(std::make_index_sequence<std::tuple_size_v<TypeList>>{})
		};

		return match_index;
	}
}

namespace roy{
	template<typename ReflectableType>
	using reflection_of = roy::provide_reflection<ReflectableType>::type;

	template<typename ReflectableType>
	using field_reflections_of = roy::reflection_of<ReflectableType>::fields::reflected_fields;

	template<auto FieldPtr>
	inline constexpr auto index_of() noexcept
		-> std::size_t{
		using fields = roy::reflection_of<roy::util::field_ptr_declaring_type_t<FieldPtr>>::fields;
		static constexpr std::size_t match_index{
			[]<std::size_t... Is>(std::index_sequence<Is...>){
				static constexpr std::array<bool, sizeof...(Is)> is_equal{
					roy::detail::field_ptrs_equal(
						roy::util::nth_type<Is, typename fields::reflected_fields>::type::field_ptr(),
						FieldPtr
					)...
				};

				static_assert((is_equal[Is] || ...), "Field not found.");
				for(std::size_t i{0}; i < sizeof...(Is); ++i){
					if(is_equal[i])
						return i;
				}

				std::unreachable();
			}(std::make_index_sequence<fields::size()>{})
		};

		return match_index;
	}

	template<typename ReflectableType>
	using type_of = roy::reflection_of<ReflectableType>::type;

	template<typename ReflectableType>
	inline constexpr auto type_name_of() noexcept{
		return roy::reflection_of<ReflectableType>::name();
	}

	template<std::size_t N, typename ReflectableType>
	using nth_field_reflection_of = roy::util::nth_type<N, roy::field_reflections_of<ReflectableType>>::type;

	template<std::size_t N, typename ReflectableType>
	using nth_field_type_of = roy::nth_field_reflection_of<N, ReflectableType>::field_type;

	template<std::size_t N, typename ReflectableType>
	using nth_field_ptr_type_of = roy::nth_field_reflection_of<N, ReflectableType>::field_ptr_type;

	template<std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_ptr_of() noexcept{
		return roy::nth_field_reflection_of<N, ReflectableType>::field_ptr();
	}

	template<std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_name_of() noexcept{
		return roy::nth_field_reflection_of<N, ReflectableType>::name();
	}

	template<typename ReflectableType>
	inline constexpr auto field_count_of() noexcept -> std::size_t{
		return roy::reflection_of<ReflectableType>::fields::size();
	}

	template<typename ReflectableType>
	inline constexpr auto field_count(ReflectableType&& obj) noexcept
		-> std::size_t{
		return roy::field_count_of<std::remove_cvref_t<ReflectableType>>;
	}

	template<std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_value(ReflectableType& obj) noexcept
		-> std::add_lvalue_reference_t<roy::nth_field_type_of<N, ReflectableType>>{
		return obj.*roy::nth_field_ptr_of<N, ReflectableType>();
	}

	template<std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_value(const ReflectableType& obj) noexcept
		-> std::add_lvalue_reference_t<std::add_const_t<roy::nth_field_type_of<N, ReflectableType>>>{
		return obj.*roy::nth_field_ptr_of<N, ReflectableType>();
	}

	template<std::size_t N, typename ReflectableType>
	inline constexpr auto annotations_of_nth_field() noexcept{
		return roy::nth_field_reflection_of<N, ReflectableType>::annotation_values;
	}

	template<typename ReflectableType>
	inline constexpr auto annotations_of() noexcept{
		return roy::reflection_of<ReflectableType>::annotation_values;
	}

	template<auto FieldPtr>
	inline constexpr auto annotations_of() noexcept{
		static constexpr std::size_t field_index{roy::index_of<FieldPtr>()};
		using field_reflection = roy::nth_field_reflection_of<
			field_index,
			roy::util::field_ptr_declaring_type_t<FieldPtr>
		>;

		return field_reflection::annotation_values;
	}

	template<typename AnnotationType, std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_has_annotation() noexcept
		-> bool{
		return roy::detail::contains_type<
			AnnotationType,
			decltype(roy::nth_field_reflection_of<N, ReflectableType>::annotation_values)
		>();
	}

	template<typename AnnotationType, typename ReflectableType>
	inline constexpr auto has_annotation() noexcept
		-> bool{
		return roy::detail::contains_type<
			AnnotationType,
			decltype(roy::annotations_of<ReflectableType>())
		>();
	}

	template<typename AnnotationType, auto FieldPtr>
	inline constexpr auto has_annotation() noexcept
		-> bool{
		return roy::detail::contains_type<
			AnnotationType,
			decltype(roy::annotations_of<FieldPtr>())
		>();
	}

	template<template<typename...> typename AnnotationType, std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_has_annotation() noexcept
		-> bool{
		return roy::detail::contains_type_templated_type<
			AnnotationType,
			std::remove_cvref_t<decltype(roy::nth_field_reflection_of<N, ReflectableType>::annotation_values)>
		>();
	}

	template<template<typename...> typename AnnotationType, typename ReflectableType>
	inline constexpr auto has_annotation() noexcept
		-> bool{
		return roy::detail::contains_type_templated_type<
			AnnotationType,
			std::remove_cvref_t<decltype(roy::annotations_of<ReflectableType>())>
		>();
	}

	template<template<typename...> typename AnnotationType, auto FieldPtr>
	inline constexpr auto has_annotation() noexcept
		-> bool{
		return roy::detail::contains_type_templated_type<
			AnnotationType,
			std::remove_cvref_t<decltype(roy::annotations_of<FieldPtr>())>
		>();
	}

	template<template<auto...> typename AnnotationType, std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_has_annotation() noexcept
		-> bool{
		return roy::detail::contains_value_templated_type<
			AnnotationType,
			std::remove_cvref_t<decltype(roy::nth_field_reflection_of<N, ReflectableType>::annotation_values)>
		>();
	}

	template<template<auto...> typename AnnotationType, typename ReflectableType>
	inline constexpr auto has_annotation() noexcept
		-> bool{
		return roy::detail::contains_value_templated_type<
			AnnotationType,
			std::remove_cvref_t<decltype(roy::annotations_of<ReflectableType>())>
		>();
	}

	template<template<auto...> typename AnnotationType, auto FieldPtr>
	inline constexpr auto has_annotation() noexcept
		-> bool{
		return roy::detail::contains_value_templated_type<
			AnnotationType,
			std::remove_cvref_t<decltype(roy::annotations_of<FieldPtr>())>
		>();
	}

	template<typename AnnotationType, std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_annotation_of() noexcept
		-> AnnotationType{
		return std::get<AnnotationType>(roy::annotations_of_nth_field<N, ReflectableType>());
	}

	template<typename AnnotationType, typename ReflectableType>
	inline constexpr auto annotation_of() noexcept
		-> AnnotationType{
		return std::get<AnnotationType>(roy::annotations_of<ReflectableType>());
	}

	template<typename AnnotationType, auto FieldPtr>
	inline constexpr auto annotation_of() noexcept
		-> AnnotationType{
		return std::get<AnnotationType>(roy::annotations_of<FieldPtr>());
	}

	template<template<typename...> typename AnnotationType, std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_annotation_of() noexcept{
		static constexpr auto annotation_values{roy::annotations_of_nth_field<N, ReflectableType>()};
		static constexpr std::size_t annotation_index{
			roy::detail::get_type_templated_type_index<AnnotationType, std::remove_cvref_t<decltype(annotation_values)>>()
		};
		return std::get<annotation_index>(annotation_values);
	}

	template<template<typename...> typename AnnotationType, typename ReflectableType>
	inline constexpr auto annotation_of() noexcept{
		static constexpr auto annotation_values{roy::annotations_of<ReflectableType>()};
		static constexpr std::size_t annotation_index{
			roy::detail::get_type_templated_type_index<AnnotationType, std::remove_cvref_t<decltype(annotation_values)>>()
		};
		return std::get<annotation_index>(annotation_values);
	}

	template<template<typename...> typename AnnotationType, auto FieldPtr>
	inline constexpr auto annotation_of() noexcept{
		static constexpr auto annotation_values{roy::annotations_of<FieldPtr>()};
		static constexpr std::size_t annotation_index{
			roy::detail::get_type_templated_type_index<AnnotationType, std::remove_cvref_t<decltype(annotation_values)>>()
		};

		return std::get<annotation_index>(annotation_values);
	}

	template<template<auto...> typename AnnotationType, std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_annotation_of() noexcept{
		static constexpr auto annotation_values{roy::annotations_of_nth_field<N, ReflectableType>()};
		static constexpr std::size_t annotation_index{
			roy::detail::get_value_templated_type_index<AnnotationType, std::remove_cvref_t<decltype(annotation_values)>>()
		};
		return std::get<annotation_index>(annotation_values);
	}

	template<template<auto...> typename AnnotationType, typename ReflectableType>
	inline constexpr auto annotation_of() noexcept{
		static constexpr auto annotation_values{roy::annotations_of<ReflectableType>()};
		static constexpr std::size_t annotation_index{
			roy::detail::get_value_templated_type_index<AnnotationType, std::remove_cvref_t<decltype(annotation_values)>>()
		};
		return std::get<annotation_index>(annotation_values);
	}

	template<template<auto...> typename AnnotationType, auto FieldPtr>
	inline constexpr auto annotation_of() noexcept{
		static constexpr auto annotation_values{roy::annotations_of<FieldPtr>()};
		static constexpr std::size_t annotation_index{
			roy::detail::get_value_templated_type_index<AnnotationType, std::remove_cvref_t<decltype(annotation_values)>>()
		};

		return std::get<annotation_index>(annotation_values);
	}
}

#endif
