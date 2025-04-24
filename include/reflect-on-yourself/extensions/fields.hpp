#ifndef H_REFLECT_ON_YOURSELF_EXTENSIONS_FIELDS
#define H_REFLECT_ON_YOURSELF_EXTENSIONS_FIELDS

#include "../util/type_wrapper.hpp"
#include "../builder/builder.hpp"
#include "../builder/extended_builder.hpp"

#include <cstddef>

namespace roy::extensions{
	struct fields_extension_tag{};
}

template<typename For>
struct roy::detail::builder<roy::extensions::fields_extension_tag, For>{
	template<typename... Fields>
	struct extension{
		struct fields{
			static constexpr std::size_t size() noexcept{ return sizeof...(Fields); }
			using reflected_fields = roy::util::type_wrapper<Fields...>;
		};
	};

	template<typename... Fields>
	using with_fields = roy::detail::extended_builder<
		For,
		roy::extensions::fields_extension_tag,
		extension<Fields...>
	>::type;
};

#endif
