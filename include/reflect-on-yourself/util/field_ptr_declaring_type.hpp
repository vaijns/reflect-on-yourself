#ifndef H_REFLECT_ON_YOURSELF_UTIL_FIELD_PTR_DECLARING_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_FIELD_PTR_DECLARING_TYPE

namespace roy::util{
	template<auto FieldPtr>
	struct field_ptr_declaring_type;

	template<typename DeclaringType, typename FieldType, FieldType DeclaringType::* FieldPtr>
	struct field_ptr_declaring_type<FieldPtr>{
		using type = DeclaringType;
	};

	template<auto FieldPtr>
	using field_ptr_declaring_type_t = roy::util::field_ptr_declaring_type<FieldPtr>::type;
}

#endif
