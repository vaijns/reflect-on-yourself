#ifndef H_REFLECT_ON_YOURSELF_REFLECTION
#define H_REFLECT_ON_YOURSELF_REFLECTION

#include "./builder/type_builder.hpp"
#include "./builder/field_builder.hpp"
#include "./extensions/name.hpp"
#include "./extensions/identifier.hpp"
#include "./extensions/fields.hpp"

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
				roy::extensions::fields_extension_tag
			>,
			roy::util::type_wrapper<>,
			roy::util::type_wrapper<>
		>;

		template<auto FieldPtr>
		using for_field = roy::detail::field_builder<
			FieldPtr,
			roy::util::type_wrapper<
				roy::extensions::name_extension_tag,
				roy::extensions::identifier_extension_tag
			>,
			roy::util::type_wrapper<>,
			roy::util::type_wrapper<>
		>;
	}
}

namespace roy{
	template<typename ReflectableType>
	using reflection_of = roy::provide_reflection<ReflectableType>::type;

	template<typename ReflectableType>
	using field_reflections_of = roy::reflection_of<ReflectableType>::fields::reflected_fields;

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
}

#endif
