#ifndef H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_FLOATING_POINT_TYPES
#define H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_FLOATING_POINT_TYPES

#include "../reflection.hpp"
#include "./builtin_type_extension.hpp"

namespace roy::identifier{
	ROY_MAKE_REFLECTION_IDENTIFIER(float32);
	ROY_MAKE_REFLECTION_IDENTIFIER(float64);
	ROY_MAKE_REFLECTION_IDENTIFIER(float128);
}

template<> struct roy::provide_reflection<float>
	: roy::reflection::for_type<float>
		::with_auto_name
		//::with_name<"float32">
		::with_identifier<roy::identifier::float32>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension, true>
		::result{};

template<> struct roy::provide_reflection<double>
	: roy::reflection::for_type<double>
		::with_auto_name
		//::with_name<"float64">
		::with_identifier<roy::identifier::float64>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension, true>
		::result{};

template<> struct roy::provide_reflection<long double>
	: roy::reflection::for_type<long double>
		::with_auto_name
		//::with_name<"float128">
		::with_identifier<roy::identifier::float128>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension, true>
		::result{};

#endif
