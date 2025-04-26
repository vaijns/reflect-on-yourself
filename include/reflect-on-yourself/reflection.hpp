#ifndef H_REFLECT_ON_YOURSELF_REFLECTION
#define H_REFLECT_ON_YOURSELF_REFLECTION

#include "./builder/type_builder.hpp"
#include "./builder/field_builder.hpp"
#include "./extensions/name.hpp"
#include "./extensions/identifier.hpp"
#include "./extensions/fields.hpp"
#include "./extensions/annotation.hpp"
#include "reflect-on-yourself/util/field_ptr_declaring_type.hpp"

namespace roy{
	template<typename T>
	struct provide_reflection;

	namespace reflection{
		template<typename Type>
		using for_type = roy::detail::type_builder<
			Type,
			roy::util::type_wrapper<
				roy::extensions::name_extension_tag,
				roy::extensions::identifier_extension_tag,
				roy::extensions::fields_extension_tag,
				roy::extensions::annotation_extension_tag
			>,
			roy::util::type_wrapper<>,
			roy::util::type_wrapper<>
		>;

		template<auto FieldPtr>
		using for_field = roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<
				roy::extensions::name_extension_tag,
				roy::extensions::identifier_extension_tag,
				roy::extensions::annotation_extension_tag
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

	template<typename AnnotationType, std::size_t N, typename ReflectableType>
	inline constexpr auto nth_field_annotation_of() noexcept
		-> AnnotationType{
		return std::get<AnnotationType>(roy::nth_field_reflection_of<N, ReflectableType>::annotation_values);
	}

	template<typename AnnotationType, typename ReflectableType>
	inline constexpr auto annotation_of() noexcept
		-> AnnotationType{
		return std::get<AnnotationType>(roy::reflection_of<ReflectableType>::annotation_values);
	}

	template<typename AnnotationType, auto FieldPtr>
	inline constexpr auto annotation_of() noexcept
		-> AnnotationType{
		static constexpr std::size_t field_index{roy::index_of<FieldPtr>()};
		using field_reflection = roy::nth_field_reflection_of<
			field_index,
			roy::util::field_ptr_declaring_type_t<FieldPtr>
		>;

		return std::get<AnnotationType>(field_reflection::annotation_values);
	}
}

#endif
