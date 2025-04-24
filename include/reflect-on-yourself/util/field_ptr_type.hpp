#ifndef H_REFLECT_ON_YOURSELF_UTIL_FIELD_PTR_TYPE
#define H_REFLECT_ON_YOURSELF_UTIL_FIELD_PTR_TYPE

namespace roy::util{
	template<auto FieldPtr>
	struct field_ptr_type;

	template<typename DeclaringType, typename FieldType, FieldType DeclaringType::* FieldPtr>
	struct field_ptr_type<FieldPtr>{
		using type = FieldType;
	};

	template<auto FieldPtr>
	using field_ptr_type_t = roy::util::field_ptr_type<FieldPtr>::type;
}

#endif
