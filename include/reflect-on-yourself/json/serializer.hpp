#ifndef H_REFLECT_ON_YOURSELF_JSON_SERIALIZER
#define H_REFLECT_ON_YOURSELF_JSON_SERIALIZER

#include "../field.hpp"
#include "../type_info.hpp"

#include <concepts>
#include <optional>
#include <ostream>
#include <ranges>
#include <sstream>
#include <utility>

namespace roy::json::detail {
	template<typename TSource>
		requires(with_type_info<TSource> && roy::type_info_for_t<TSource>::fields::size() > 0)
	inline void serialize(std::ostream& stream, const TSource& obj);

	inline void serialize(std::ostream& stream, std::integral auto value);

	inline void serialize(std::ostream& stream, std::floating_point auto value);

	inline void serialize(std::ostream& stream, bool value);

	inline void serialize(std::ostream& stream, const std::string& value);

	template<typename T>
	inline void serialize(std::ostream& stream, const std::optional<T>& value);

	template<typename TField>
		requires(field_type<TField>)
	inline void serialize(std::ostream& stream, const typename TField::type& obj);

	template<typename TTypeInfo, std::size_t... Is>
		requires(type_info_type<TTypeInfo>)
	inline void serialize(std::ostream& stream, const typename TTypeInfo::type& obj, std::index_sequence<Is...>);

	template<typename TTypeInfo>
		requires(type_info_type<TTypeInfo>)
	inline void serialize(std::ostream& stream, const typename TTypeInfo::type& obj);

	template<std::ranges::forward_range Range>
	inline void serialize(std::ostream& stream, Range&& r);

	template<typename TSource>
		requires(with_type_info<TSource> && roy::type_info_for_t<TSource>::fields::size() > 0)
	inline void serialize(std::ostream& stream, const TSource& obj) {
		using type_info = roy::type_info_for_t<TSource>;
		detail::serialize<type_info>(stream, obj);
	}

	inline void serialize(std::ostream& stream, std::integral auto value) {
		stream << value;
	}

	inline void serialize(std::ostream& stream, std::floating_point auto value) {
		stream << value;
	}

	inline void serialize(std::ostream& stream, bool value) {
		if(value)
			stream << "true";
		else
			stream << "false";
	}

	inline void serialize(std::ostream& stream, const std::string& value) {
		stream << '"' << value << '"';
	}

	template<typename T>
	inline void serialize(std::ostream& stream, const std::optional<T>& value) {
		if(value.has_value())
			detail::serialize(stream, value.value());
		else
			stream << "null";
	}

	template<typename TField>
		requires(field_type<TField>)
	inline void serialize(std::ostream& stream, const typename TField::type& obj) {
		using field_type_info = roy::type_info_for_t<typename TField::type>;
		if constexpr(field_type_info::fields::size() == 0) {
			stream << '"' << TField::name << '"' << ':';
			detail::serialize(stream, obj);
		} else {
			stream << '"' << TField::name << '"' << ':' << '{';
			detail::serialize<field_type_info>(
				stream, obj, std::make_index_sequence<field_type_info::fields::size()>{}
			);
			stream << '}';
		}
	}

	template<typename TTypeInfo, std::size_t... Is>
		requires(type_info_type<TTypeInfo>)
	inline void serialize(std::ostream& stream, const typename TTypeInfo::type& obj, std::index_sequence<Is...>) {
		bool first = true;
		(
			[&]() {
				if(not first)
					stream << ',';

				first = false;
				using field_type = TTypeInfo::fields::template nth_type<Is>;
				detail::serialize<field_type>(stream, obj.*field_type::pointer);
			}(),
			...
		);
	}

	template<typename TTypeInfo>
		requires(type_info_type<TTypeInfo>)
	inline void serialize(std::ostream& stream, const typename TTypeInfo::type& obj) {
		detail::serialize<TTypeInfo>(stream, obj, std::make_index_sequence<TTypeInfo::fields::size()>{});
	}

	template<std::ranges::forward_range Range>
	inline void serialize(std::ostream& stream, Range&& r) {
		bool first = true;
		stream << '[';
		for(const auto& item : r) {
			if(not first)
				stream << ',';
			first = false;
			detail::serialize(stream, item);
		}
		stream << ']';
	}
} // namespace roy::json::detail

namespace roy::json {
	template<typename TSource>
		requires(with_type_info<TSource>)
	std::string serialize(const TSource& obj) {
		using type_info = roy::type_info_for_t<TSource>;

		std::stringstream s{};
		s << '{';
		detail::serialize<type_info>(s, obj);
		s << '}';
		return s.str();
	}
} // namespace roy::json
#endif
