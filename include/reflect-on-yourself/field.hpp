#ifndef H_REFLECT_ON_YOURSELF_FIELD
#define H_REFLECT_ON_YOURSELF_FIELD

#include "./util.hpp"

namespace roy{
	template<auto FieldPtr, util::basic_string_literal Name/*, typename FlagType, FlagType Flags*/>
		requires(util::member_field_pointer<FieldPtr>)
	struct field{
		using type = util::type_of_member_field_pointer_t<FieldPtr>;
		static constexpr auto name = Name;
		static constexpr auto pointer = FieldPtr;
	};
}
#endif
