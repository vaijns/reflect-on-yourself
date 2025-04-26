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

struct user{
	std::uint64_t id;
	std::string email;
	std::optional<std::string> name;
};

struct table_name{
	const char* name;
};
static constexpr const char user_table_name[]{"user_table"};

struct other_annotation{
	int value;
};

struct column_name{
	const char* name;
};
static constexpr const char id_column_name[]{"id"};
static constexpr const char email_column_name[]{"mail"};
static constexpr const char name_column_name[]{"name"};

template<> struct roy::provide_reflection<user>
	: roy::reflection::for_type<user>
		::with_auto_name
		::with_fields<
			roy::reflection::for_field<&user::id>
				::with_auto_name
				::with_annotation<column_name{id_column_name}>
				::result,
			roy::reflection::for_field<&user::email>
				::with_auto_name
				::with_annotation<column_name{email_column_name}>
				::result,
			roy::reflection::for_field<&user::name>
				::with_auto_name
				::with_annotation<column_name{name_column_name}>
				::result
		>
		::with_annotation<table_name{user_table_name}>
		::with_annotation<other_annotation{3}>
		::result{};

template<typename T>
concept has_reflection = requires(T t){
	typename roy::reflection_of<T>;
};

template<typename T>
concept reflection_of_builtin = has_reflection<T> && roy::reflection_of<T>::is_builtin_type();

template<typename T> requires(has_reflection<T> && not reflection_of_builtin<T>)
std::string json_serialize(const T& t);

std::string json_serialize(const std::string& val){
	return std::format("\"{}\"", val);
}

std::string json_serialize(std::string_view val){
	return std::format("\"{}\"", val);
}

std::string json_serialize(std::integral auto val){
	return std::to_string(val);
}

std::string json_serialize(std::floating_point auto val){
	return std::to_string(val);
}

std::string json_serialize(bool val){
	if(val)
		return std::string{"true"};

	return std::string{"false"};
}

template<typename T>
std::string json_serialize(const std::optional<T>& t){
	if(not t.has_value())
		return std::string{"null"};

	return json_serialize(t.value());
}

template<std::ranges::forward_range R>
std::string json_serialize(R&& r){
	std::stringstream s{};
	s << '[';
	bool is_first{true};
	for(const auto& t : r){
		if(not is_first)
			s << ',';

		s << json_serialize(t);

		is_first = false;
	}
	s << ']';

	return s.str();
}

template<typename T, std::size_t N>
std::string json_serialize_nth_field(const T& t){
	std::stringstream s{};
	if constexpr (N > 0)
		s << ',';

	s << '"' << std::string{roy::nth_field_name_of<N, T>()} << "\":" << json_serialize(roy::nth_field_value<N>(t));
	return s.str();
}

template<typename T> requires(has_reflection<T> && not reflection_of_builtin<T>)
std::string json_serialize(const T& t){
	std::stringstream s{};
	s << '{';

	[&s, &t]<std::size_t... Is>(std::index_sequence<Is...>){
		s << (json_serialize_nth_field<T, Is>(t) + ...);
	}(std::make_index_sequence<roy::field_count_of<T>()>{});

	s << '}';
	return s.str();
}

int main(int argc, char* argv[]){
	std::println("{}", roy::reflection_of<user>::name());
	std::println("{} (builtin: {})", roy::reflection_of<bool>::name(), roy::reflection_of<bool>::is_builtin_type());

	user user_a{
		.id = 1,
		.email = "email_a",
		.name = std::nullopt
	};

	const user user_b{
		.id = 1,
		.email = "email_b",
		.name = std::nullopt
	};

	std::string& email_a{roy::nth_field_value<1>(user_a)};
	const std::string& email_b{roy::nth_field_value<1>(user_b)};

	std::println("a: {}", email_a);
	std::println("b: {}", email_b);

	user_a.email = "email_a_changed";
	std::println("a: {}", email_a);

	std::println("\"users\":{}",  json_serialize(std::vector{
		user{
			.id = 0,
			.email = "abc",
			.name = "def"
		},
		user{
			.id = 1,
			.email = "ghi",
			.name = std::nullopt
		},
		user{
			.id = 2,
			.email = "jkl",
			.name = "mno"
		},
		user{
			.id = 3,
			.email = "hello@world.com",
			.name = std::nullopt
		}
	}));

	std::println("table_name: {}", roy::annotation_of<table_name, user>().name);
	std::println("other_annotation: {}", roy::annotation_of<other_annotation, user>().value);
	std::println("column_name by index: {}", roy::nth_field_annotation_of<column_name, 1, user>().name);
	std::println("column_name by ptr: {}", roy::annotation_of<column_name, &user::id>().name);

	static_assert(roy::has_annotation<table_name, user>());
	static_assert(not roy::has_annotation<column_name, user>());

	static_assert(not roy::has_annotation<table_name, &user::email>());
	static_assert(roy::has_annotation<column_name, &user::email>());

	static_assert(not roy::nth_field_has_annotation<table_name, 0, user>());
	static_assert(roy::nth_field_has_annotation<column_name, 0, user>());

	return 0;
}
