#include "reflect-on-yourself/reflection.hpp"
#include <reflect-on-yourself.hpp>
#include <print>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <array>

struct user{
	std::uint64_t id;
	std::string email;
	std::optional<std::string> name;
};

template<> struct roy::provide_reflection<user>
	: roy::reflection::for_type<user>
		::with_name<"user">
		::with_fields<
			roy::reflection::for_field<&user::id>
				::with_name<"id">
				::result,
			roy::reflection::for_field<&user::email>
				::with_name<"email">
				::result,
			roy::reflection::for_field<&user::name>
				::with_name<"name">
				::result
		>
		::result{};

std::string to_string(std::uint64_t value){
	return std::to_string(value);
}

std::string to_string(std::string&& value){
	return value;
}

std::string to_string(const std::string& value){
	return value;
}

std::string to_string(const std::optional<std::string>& value){
	if(not value.has_value())
		return std::string{"-"};

	return value.value();
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

	auto key_value_pairs{[user_a]<std::size_t... Is>(std::index_sequence<Is...>){
		return std::array<std::pair<std::string, std::string>, sizeof...(Is)>{
			std::pair<std::string, std::string>{
				std::string{roy::nth_field_name_of<Is, user>()},
				to_string(roy::nth_field_value<Is>(user_a))
			}...
		};
	}(std::make_index_sequence<roy::field_count_of<user>()>{})};

	for(const auto& [field_name, field_value] : key_value_pairs){
		std::println("{}: {}", field_name, field_value);
	}

	return 0;
}
