#ifndef H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_BOOL_TYPE
#define H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_BOOL_TYPE

#include "../reflection.hpp"

namespace roy::identifier{
	ROY_MAKE_REFLECTION_IDENTIFIER(boolean);
}

template<> struct roy::provide_reflection<bool>
	: roy::reflection::for_type<bool>
		::with_name<"boolean">
		::with_identifier<roy::identifier::boolean>
		::result{};

#endif
