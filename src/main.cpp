#include <iostream>
#include <optional>
#include <cstdint>
#include <cstddef>
#include <string>
#include <utility>
#include <string_view>
#include <variant>
#include "../include/reflect-on-yourself/type_info.hpp"

struct group{
	std::uint64_t id;
	std::string name;
	std::optional<std::string> description;
};
struct user{
	group user_group;
	std::uint64_t id;
	std::string email;
	std::optional<std::string> name;
	std::string password;
	bool is_verified;
};

namespace roy{
	template<> struct type_info_for<group>
		: roy::type_info_provider<group, roy::type_info<group, "group",
			roy::util::type_wrapper<
				roy::field<&group::id, "id">,
				roy::field<&group::name, "name">,
				roy::field<&group::description, "description">
			>
		>
	>{};
	template<> struct type_info_for<user>
		: roy::type_info_provider<user, roy::type_info<user, "user",
			roy::util::type_wrapper<
				roy::field<&user::user_group, "group">,
				roy::field<&user::id, "id">,
				roy::field<&user::email, "email">,
				roy::field<&user::name, "name">,
				roy::field<&user::password, "password">,
				roy::field<&user::is_verified, "is_verified">
			>
		>::alias<"testing">::fields::template alias<&user::email, "e_mail">
	>{};
}

template<typename T, typename Obj>
void print_field(const Obj& obj, const std::string& prefix = "");

template<typename T, typename Obj, std::size_t... Is>
void print_type(const Obj& obj, std::index_sequence<Is...>, const std::string& prefix = "");

void print_value(std::uint64_t value){ std::cout << value; }
void print_value(const std::string& value){ std::cout << '"' << value << '"'; }
void print_value(bool value){ std::cout << (value ? "true" : "false"); }
void print_value(std::nullopt_t){
	std::cout << "null";
}
template<typename T> void print_value(const std::optional<T>& value){
	if(value.has_value())
		print_value(value.value());
	else
		print_value(std::nullopt);
}

template<typename T>
void print(const T& t);

template<typename T, typename Obj, std::size_t... Is>
void print_type(const Obj& obj, std::index_sequence<Is...>, const std::string& prefix){
	(print_field<std::remove_cvref_t<typename T::fields::template nth_type<Is>>>(obj, prefix), ...);
}

template<typename T, typename Obj>
void print_field(const Obj& obj, const std::string& prefix){
	using type_info = roy::type_info_for_t<typename T::type>;
	std::cout << prefix << "(" << type_info::name << ") " << T::name.data() << " = ";

	if constexpr(type_info::fields::size() == 0){
		print_value(obj.*T::pointer);
		std::cout << "\n";
	} else{
		std::cout << "{\n";
		print_type<type_info>((obj.*T::pointer), std::make_index_sequence<type_info::fields::size()>{}, prefix + '\t');
		std::cout << prefix << "}\n";
	}
}

template<typename T>
void print(const T& t){
	using type_info = roy::type_info_for_t<T>;
	std::cout << type_info::name.data() << "{\n";
	print_type<type_info>(t, std::make_index_sequence<type_info::fields::size()>{}, "\t");
	std::cout << "}\n";
}

int main(){
	print(user{
		.user_group = {
			.id = 2,
			.name = "my_group",
			.description = std::nullopt
		},
		.id = 3,
		.email = "hello@world.com",
		.name = "my_name",
		.password = "my_password",
		.is_verified = true
	});
	print(user{
		.user_group = {
			.id = 5,
			.name = "other_group",
			.description = "yep"
		},
		.id = 7,
		.email = "goodbye@everyone.com",
		.name = "other_name",
		.password = "pw",
		.is_verified = false
	});
}
