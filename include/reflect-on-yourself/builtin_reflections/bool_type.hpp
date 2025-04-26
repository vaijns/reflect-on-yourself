#ifndef H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_BOOL_TYPE
#define H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_BOOL_TYPE

#include "../reflection.hpp"
#include "./builtin_type_extension.hpp"

namespace roy::identifier{
	ROY_MAKE_REFLECTION_IDENTIFIER(boolean);
}

template<> struct roy::provide_reflection<bool>
	: roy::reflection::for_type<bool>
		::with_auto_name
		//::with_name<"boolean">
		::with_identifier<roy::identifier::boolean>
		::extend<roy::detail::builtin_type_extension_tag, roy::detail::builtin_type_extension>
		::result{};

#endif
