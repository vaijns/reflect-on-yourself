#ifndef H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_BUILTIN_TYPE_EXTENSION
#define H_REFLECT_ON_YOURSELF_BUILTIN_REFLECTIONS_BUILTIN_TYPE_EXTENSION

namespace roy::detail{
	struct builtin_type_extension_tag{};

	struct builtin_type_extension{
		static constexpr auto is_builtin_type() noexcept -> bool { return true; }
	};
}

#endif
