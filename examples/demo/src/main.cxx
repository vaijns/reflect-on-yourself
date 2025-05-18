#include <reflect-on-yourself.hpp>
#include <print>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <ranges>
#include <sstream>
#include <format>
#include <fstream>

#include <reflect-on-yourself/serialization.hpp>

inline constexpr auto idk{roy::util::inplace_string{"abc"}};

struct child_object{
	std::string name;
};

struct user{
	std::uint64_t id;
	std::string email;
	std::optional<std::string> name;
	child_object child;
};

template<std::size_t N> struct table_name{ const char name[N]; };
struct other_annotation{ int value; };
template<std::size_t N> struct column_name{ const char name[N]; };
template<std::size_t NA, std::size_t NB, std::size_t NC> struct multiple_strings{
	const char string_a[NA];
	const char string_b[NB];
	const char string_c[NC];
};

template<> struct roy::provide_reflection<child_object>
	: roy::reflection::for_type<child_object>
		::with_default_builders
		::with_auto_name
		::with_annotation<roy::serialization::strategy::as_object>
		::with_fields<
			roy::reflection::for_field<&child_object::name>
				::with_default_builders
				::with_auto_name
				::result
		>
		::result{};

template<> struct roy::provide_reflection<user>
	: roy::reflection::for_type<user>
		::with_default_builders
		::with_auto_name
		::with_annotation<roy::serialization::strategy::as_object>
		::with_fields<
			roy::reflection::for_field<&user::id>
				::with_default_builders
				::with_auto_name
				::with_annotation<column_name{"id"}>
				::result,
			roy::reflection::for_field<&user::email>
				::with_default_builders
				::with_auto_name
				::with_annotation<column_name{"e-mail"}>
				::result,
			roy::reflection::for_field<&user::name>
				::with_default_builders
				::with_auto_name
				::with_annotation<column_name{"name"}>
				::with_annotation<multiple_strings{"string_a", "another string", "string #c"}>
				::result,
			roy::reflection::for_field<&user::child>
				::with_default_builders
				::with_auto_name
				::result
		>
		::with_annotation<table_name{"user_table"}>
		::with_annotation<other_annotation{3}>
		::result{};

template<typename T> struct roy::provide_reflection<std::vector<T>>
	: roy::reflection::for_type<std::vector<T>>
		::with_default_builders
		::with_auto_name
		::template with_annotation<roy::serialization::strategy::as_array>
		::result{};

template<typename T> struct roy::provide_reflection<std::span<T>>
	: roy::reflection::for_type<std::span<T>>
		::with_default_builders
		::with_auto_name
		::template with_annotation<roy::serialization::strategy::as_array>
		::result{};

template<typename T, std::size_t N> struct roy::provide_reflection<std::array<T, N>>
	: roy::reflection::for_type<std::array<T, N>>
		::with_default_builders
		::with_auto_name
		::template with_annotation<roy::serialization::strategy::as_array>
		::result{};

template<typename T>
auto json_serialize(T&& data) -> std::string{
	return roy::serialization::serialize<roy::serialization::stringstream_sink, roy::serialization::json_serializer>(
		std::forward<T>(data),
		roy::serialization::stringstream_sink::settings_type{},
		roy::serialization::json_settings{}
	);
}

template<typename T>
auto json_serialize(std::filesystem::path&& path, T&& data) -> void{
	roy::serialization::serialize<roy::serialization::file_sink, roy::serialization::json_serializer>(
		std::forward<T>(data),
		std::filesystem::path{path},
		roy::serialization::json_settings{
			.indent_char{'\t'},
			.chars_per_indent{1}
		}
	);
}

template<typename T>
auto xml_serialize(T&& data) -> std::string{
	return roy::serialization::serialize<roy::serialization::stringstream_sink, roy::serialization::xml_serializer>(
		std::forward<T>(data),
		roy::serialization::stringstream_sink::settings_type{},
		roy::serialization::xml_settings{}
	);
}

template<typename T>
auto xml_serialize(std::filesystem::path&& path, T&& data) -> void{
	roy::serialization::serialize<roy::serialization::file_sink, roy::serialization::xml_serializer>(
		std::forward<T>(data),
		std::filesystem::path{path},
		roy::serialization::xml_settings{
			.indent_char{'\t'},
			.chars_per_indent{1}
		}
	);
}

auto main(int /*argc*/, char* /*argv*/[]) -> int{
	std::println("{}", roy::reflection_of<user>::name());
	std::println("{} (builtin: {})", roy::reflection_of<bool>::name(), roy::reflection_of<bool>::is_builtin_type());

	user user_a{
		.id = 1,
		.email = "email_a",
		.name = std::nullopt,
		.child = {
			.name = "this is a child object"
		}
	};

	const user user_b{
		.id = 1,
		.email = "email_b",
		.name = std::nullopt,
		.child = {
			.name = "nice"
		}
	};

	std::string& email_a{roy::nth_field_value<1>(user_a)};
	const std::string& email_b{roy::nth_field_value<1>(user_b)};

	std::println("a: {}", email_a);
	std::println("b: {}", email_b);

	user_a.email = "email_a_changed";
	std::println("a: {}", email_a);

	std::println("table_name: {}", roy::annotation_of<table_name, user>().name);
	std::println("other_annotation: {}", roy::annotation_of<other_annotation, user>().value);
	std::println("column_name by index: {}", roy::nth_field_annotation_of<column_name, 1, user>().name);
	std::println("column_name by ptr: {}", roy::annotation_of<column_name, &user::id>().name);
	std::println(
		"multiple strings annotation: {}, {}, {}",
		roy::annotation_of<multiple_strings, &user::name>().string_a,
		roy::annotation_of<multiple_strings, &user::name>().string_b,
		roy::annotation_of<multiple_strings, &user::name>().string_c
	);

	static_assert(roy::has_annotation<table_name, user>());
	static_assert(not roy::has_annotation<column_name, user>());

	static_assert(not roy::has_annotation<table_name, &user::email>());
	static_assert(roy::has_annotation<column_name, &user::email>());

	static_assert(not roy::nth_field_has_annotation<table_name, 0, user>());
	static_assert(roy::nth_field_has_annotation<column_name, 0, user>());

	roy::reflection_of<std::uint64_t>::identifier::field<std::string> my_field{};
	my_field.uint64 = "identifier_field";

	std::println("serialized string: {}", json_serialize("> hello world!!!!"));

	std::println("serialized user: {}", json_serialize(user_a));

	json_serialize(std::filesystem::path{"./output/test.json"}, std::vector{
		user{
			.id = 0,
			.email = "abc",
			.name = "def",
			.child = {
				.name = "child"
			}
		},
		user{
			.id = 1,
			.email = "ghi",
			.name = std::nullopt,
			.child = {
				.name = "abc"
			}
		},
		user{
			.id = 2,
			.email = "jkl",
			.name = "mno",
			.child = {
				.name = "def"
			}
		},
		user{
			.id = 3,
			.email = "hello@world.com",
			.name = std::nullopt,
			.child = {
				.name = "ghi"
			}
		}
	});

	xml_serialize(std::filesystem::path{"./output/test.xml"}, std::vector{
		user{
			.id = 0,
			.email = "abc",
			.name = "def",
			.child = {
				.name = "child"
			}
		},
		user{
			.id = 1,
			.email = "ghi",
			.name = std::nullopt,
			.child = {
				.name = "abc"
			}
		},
		user{
			.id = 2,
			.email = "jkl",
			.name = "mno",
			.child = {
				.name = "def"
			}
		},
		user{
			.id = 3,
			.email = "hello@world.com",
			.name = std::nullopt,
			.child = {
				.name = "ghi"
			}
		}
	});

	std::println(
		"user size: {}",
		roy::serialization::serialize<roy::serialization::size_counter_sink, roy::serialization::json_serializer>(
			user_a,
			roy::serialization::size_counter_sink::settings_type{},
			roy::serialization::json_settings{}
		)
	);

	return 0;
}
