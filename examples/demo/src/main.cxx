#include "reflect-on-yourself/reflection.hpp"
#include <reflect-on-yourself.hpp>
#include <print>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <array>
#include <vector>
#include <ranges>
#include <sstream>

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

template<typename T>
concept has_reflection = requires(T t){
	typename roy::reflection_of<T>;
};

template<typename T>
concept reflection_of_builtin = has_reflection<T> && roy::reflection_of<T>::is_builtin_type();

template<typename T> requires(has_reflection<T> && not reflection_of_builtin<T>)
constexpr std::string json_serialize(const T& t);

constexpr std::string json_serialize(const std::string& val){
	std::stringstream s{};
	s << '"' << val << '"';
	return s.str();
}

constexpr std::string json_serialize(std::string_view val){
	std::stringstream s{};
	s << '"' << std::string{val} << '"';
	return s.str();
}

constexpr std::string json_serialize(std::uint8_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(std::uint16_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(std::uint32_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(std::uint64_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(std::int8_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(std::int16_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(std::int32_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(std::int64_t val){
	return std::to_string(val);
}

constexpr std::string json_serialize(bool val){
	if(val)
		return std::string{"true"};

	return std::string{"false"};
}

constexpr std::string json_serialize(float val){
	return std::to_string(val);
}

constexpr std::string json_serialize(double val){
	return std::to_string(val);
}

constexpr std::string json_serialize(long double val){
	return std::to_string(val);
}

template<typename T>
constexpr std::string json_serialize(const std::optional<T>& t){
	if(not t.has_value())
		return std::string{"null"};

	return json_serialize(t.value());
}

template<std::ranges::forward_range R>
constexpr std::string json_serialize(R&& r){
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
constexpr std::string json_serialize_nth_field(const T& t){
	std::stringstream s{};
	if constexpr (N > 0)
		s << ',';

	s << '"' << std::string{roy::nth_field_name_of<N, T>()} << "\":" << json_serialize(roy::nth_field_value<N>(t));
	return s.str();
}

template<typename T> requires(has_reflection<T> && not reflection_of_builtin<T>)
constexpr std::string json_serialize(const T& t){
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
			.id = 3,
			.email = "hello@world.com",
			.name = std::nullopt
		}
	}));

	return 0;
}
