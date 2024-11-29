#include "../include/reflect-on-yourself/json.hpp"
#include "../include/reflect-on-yourself/type_info.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

struct sub_type {
	std::optional<std::optional<std::string>> some_field;
};

struct group {
	sub_type member;
	std::uint64_t id;
	std::string name;
	std::optional<std::string> description;
};

struct user {
	group user_group;
	std::uint64_t id;
	std::string email;
	std::optional<std::string> name;
	std::string password;
	bool is_verified;
	std::vector<double> other;
};

namespace roy {
	template<>
	struct type_info_for<sub_type>
		: roy::type_info_provider<sub_type, roy::type_info<sub_type, "sub_type", roy::util::type_wrapper<roy::field<&sub_type::some_field, "some_field">>>> {
	};

	template<>
	struct type_info_for<group>
		: roy::type_info_provider<
			  group,
			  roy::type_info<group, "group", roy::util::type_wrapper<roy::field<&group::member, "member">, roy::field<&group::id, "id">, roy::field<&group::name, "name">, roy::field<&group::description, "description">>>> {
	};

	template<>
	struct type_info_for<user> : roy::type_info_provider<
					     user,
					     roy::type_info<
						     user,
						     "user",
						     roy::util::type_wrapper<
							     roy::field<&user::user_group, "group">,
							     roy::field<&user::id, "id">,
							     roy::field<&user::email, "email">,
							     roy::field<&user::name, "name">,
							     roy::field<&user::password, "password">,
							     roy::field<&user::is_verified, "is_verified">,
							     roy::field<&user::other, "other">>>> { };
} // namespace roy

int main() {
	std::cout << roy::json::serialize(user{
		.user_group = {.member = {.some_field = "hehe"}, .id = 5, .name = "other_group", .description = std::nullopt},
		.id = 7,
		.email = "goodbye@everyone.com",
		.name = "other_name",
		.password = "pw",
		.is_verified = false,
		.other = {0.5, 0.1, 12.3, 5, 9}
	});
}
