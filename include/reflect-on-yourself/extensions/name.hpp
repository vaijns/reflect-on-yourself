#ifndef H_REFLECT_ON_YOURSELF_EXTENSIONS_NAME
#define H_REFLECT_ON_YOURSELF_EXTENSIONS_NAME

#include "../builder/builder.hpp"
#include "../builder/extended_builder.hpp"
#include "../util/inplace_string.hpp"
#include "../util/get_name.hpp"

namespace roy::extensions{
	struct name_extension_tag{};
}

template<typename For>
struct roy::detail::builder<roy::extensions::name_extension_tag, For>{
	template<roy::util::inplace_string Name>
	struct extension{
		static constexpr auto name() noexcept -> decltype(Name){ return Name; }
	};

	template<roy::util::inplace_string Name>
	using with_name = roy::detail::extended_builder<
		For,
		roy::extensions::name_extension_tag,
		extension<Name>,
		true
	>::type;

	using with_auto_name = roy::detail::extended_builder<
		For,
		roy::extensions::name_extension_tag,
		extension<roy::util::get_name<For>()>,
		true
	>::type;
};
#endif
