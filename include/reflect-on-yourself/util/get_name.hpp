#ifndef H_REFLECT_ON_YOURSELF_UTIL_GET_NAME
#define H_REFLECT_ON_YOURSELF_UTIL_GET_NAME

#include <ranges>
#include <string_view>
#include <cstddef>
#include <iterator>
#include <expected>
#include <cstdint>
#include <format>
#include <source_location>
#include <utility>
#include <vector>
#include <span>
#include <cmath>
#include <print>

#include "./inplace_string.hpp"
#include "../builder/type_builder.hpp"
#include "../builder/field_builder.hpp"

namespace roy::detail{
	template<typename Type>
	constexpr auto get_type_name_signature() noexcept
		-> std::string_view{
		return std::string_view{std::source_location::current().function_name()};
	}
	template<auto FieldPtr>
	constexpr auto get_member_field_name_signature() noexcept
		-> std::string_view{
		return std::string_view{std::source_location::current().function_name()};
	}
	template<auto FunctionPtr>
	constexpr auto get_member_function_name_signature() noexcept
		-> std::string_view{
		return std::string_view{std::source_location::current().function_name()};
	}
}

namespace roy::detail::signature_parser{
	inline constexpr std::size_t error_width{230};

	enum struct lexer_token_type : std::uint8_t{
		keyword_auto,
		keyword_constexpr,
		keyword_const,
		keyword_class,
		keyword_struct,
		keyword_cdecl,
		keyword_noexcept,
		keyword_true,
		keyword_false,
		keyword_nullptr,
		keyword_with,
		string,
		character,
		floating_point,
		integer,
		identifier,
		colon,
		double_colon,
		comma,
		ampersand,
		double_ampersand,
		equals,
		semicolon,
		open_angled_brackets,
		close_angled_brackets,
		open_brackets,
		close_brackets,
		open_parentheses,
		close_parentheses,
		eof
	};
}

namespace roy::detail{
	inline auto to_string(roy::detail::signature_parser::lexer_token_type type)
		-> std::string{
		switch(type){
		case roy::detail::signature_parser::lexer_token_type::keyword_auto:
			return "auto";
		case roy::detail::signature_parser::lexer_token_type::keyword_constexpr:
			return "constexpr";
		case roy::detail::signature_parser::lexer_token_type::keyword_const:
			return "const";
		case roy::detail::signature_parser::lexer_token_type::keyword_class:
			return "class";
		case roy::detail::signature_parser::lexer_token_type::keyword_struct:
			return "struct";
		case roy::detail::signature_parser::lexer_token_type::keyword_cdecl:
			return "__cdecl";
		case roy::detail::signature_parser::lexer_token_type::keyword_noexcept:
			return "noexcept";
		case roy::detail::signature_parser::lexer_token_type::keyword_true:
			return "true";
		case roy::detail::signature_parser::lexer_token_type::keyword_false:
			return "false";
		case roy::detail::signature_parser::lexer_token_type::keyword_nullptr:
			return "nullptr";
		case roy::detail::signature_parser::lexer_token_type::keyword_with:
			return "with";
		case roy::detail::signature_parser::lexer_token_type::string:
			return "string";
		case roy::detail::signature_parser::lexer_token_type::character:
			return "char";
		case roy::detail::signature_parser::lexer_token_type::floating_point:
			return "float";
		case roy::detail::signature_parser::lexer_token_type::integer:
			return "int";
		case roy::detail::signature_parser::lexer_token_type::identifier:
			return "identifier";
		case roy::detail::signature_parser::lexer_token_type::colon:
			return ":";
		case roy::detail::signature_parser::lexer_token_type::double_colon:
			return "::";
		case roy::detail::signature_parser::lexer_token_type::comma:
			return ",";
		case roy::detail::signature_parser::lexer_token_type::ampersand:
			return "&";
		case roy::detail::signature_parser::lexer_token_type::double_ampersand:
			return "&&";
		case roy::detail::signature_parser::lexer_token_type::equals:
			return "=";
		case roy::detail::signature_parser::lexer_token_type::semicolon:
			return ";";
		case roy::detail::signature_parser::lexer_token_type::open_angled_brackets:
			return "<";
		case roy::detail::signature_parser::lexer_token_type::close_angled_brackets:
			return ">";
		case roy::detail::signature_parser::lexer_token_type::open_brackets:
			return "[";
		case roy::detail::signature_parser::lexer_token_type::close_brackets:
			return "]";
		case roy::detail::signature_parser::lexer_token_type::open_parentheses:
			return "(";
		case roy::detail::signature_parser::lexer_token_type::close_parentheses:
			return ")";
		case roy::detail::signature_parser::lexer_token_type::eof:
			return "eof";
		default:
			return "";
		}
	}
}

namespace roy::detail::signature_parser{
	struct lexer_token{
		std::string_view input;
		std::size_t position;
		std::size_t length;
		roy::detail::signature_parser::lexer_token_type type;
	};

	inline constexpr auto lexeme(roy::detail::signature_parser::lexer_token token) noexcept
		-> std::string_view{
		return token.input.substr(token.position, token.length);
	}
}

namespace roy::detail{
	inline auto to_string(roy::detail::signature_parser::lexer_token token)
		-> std::string{
		if(token.type == roy::detail::signature_parser::lexer_token_type::identifier)
			return std::format(
				"identifier \"{}\" at #{}",
				roy::detail::signature_parser::lexeme(token),
				token.position
			);

		return std::format(
			"{} at #{}",
			roy::detail::to_string(token.type),
			token.position
		);
	}
}

namespace roy::detail::signature_parser{
	enum struct lexer_error_type : std::uint8_t{
		unexpected_character
	};
}

namespace roy::detail{
	inline auto to_string(roy::detail::signature_parser::lexer_error_type type)
		-> std::string{
		switch(type){
		case roy::detail::signature_parser::lexer_error_type::unexpected_character:
			return "unexpected character";
		default:
			return "";
		}
	}
}

namespace roy::detail::signature_parser{
	struct lexer_error{
		std::string_view input;
		std::size_t position;
		roy::detail::signature_parser::lexer_error_type type;
	};
}

namespace roy::detail{
	inline auto to_string(roy::detail::signature_parser::lexer_error error)
		-> std::string{
		return std::format(
			"error: {} at (position #{} - line {}):\n{}\n{}",
			roy::detail::to_string(error.type),
			error.position + 1,
			static_cast<std::size_t>(std::floor(static_cast<double>(error.position + 1) / roy::detail::signature_parser::error_width)) + 1,
			error.input | std::views::chunk(roy::detail::signature_parser::error_width) | std::views::join_with('\n') | std::ranges::to<std::string>(),
			std::string(error.position % roy::detail::signature_parser::error_width, ' ') + '^'
		);
	}

	inline auto to_string(std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error> token)
		-> std::string{
		if(token.has_value())
			return roy::detail::to_string(token.value());

		return roy::detail::to_string(token.error());
	}
}

namespace roy::detail::signature_parser{
	class lexer{
	public:
		constexpr lexer(std::string_view input)
			: m_input{input}{

		}

		constexpr auto next_token() noexcept
			-> std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error>{
			m_token_start_index = m_current_index;

			return is_at_end()
				? make_token(roy::detail::signature_parser::lexer_token_type::eof)
				: advance_unchecked('\0')
					? make_token(roy::detail::signature_parser::lexer_token_type::eof)
				: skip_whitespace()
					? next_token()
				: advance_unchecked(':')
					? advance(':')
						? make_token(roy::detail::signature_parser::lexer_token_type::double_colon)
					: make_token(roy::detail::signature_parser::lexer_token_type::colon)
				: advance_unchecked(';')
					? make_token(roy::detail::signature_parser::lexer_token_type::semicolon)
				: advance_unchecked(',')
					? make_token(roy::detail::signature_parser::lexer_token_type::comma)
				: advance_unchecked('&')
					? advance('&')
						? make_token(roy::detail::signature_parser::lexer_token_type::double_ampersand)
					: make_token(roy::detail::signature_parser::lexer_token_type::ampersand)
				: advance_unchecked('=')
					? make_token(roy::detail::signature_parser::lexer_token_type::equals)
				: advance_unchecked('<')
					? make_token(roy::detail::signature_parser::lexer_token_type::open_angled_brackets)
				: advance_unchecked('>')
					? make_token(roy::detail::signature_parser::lexer_token_type::close_angled_brackets)
				: advance_unchecked('[')
					? make_token(roy::detail::signature_parser::lexer_token_type::open_brackets)
				: advance_unchecked(']')
					? make_token(roy::detail::signature_parser::lexer_token_type::close_brackets)
				: advance_unchecked('(')
					? make_token(roy::detail::signature_parser::lexer_token_type::open_parentheses)
				: advance_unchecked(')')
					? make_token(roy::detail::signature_parser::lexer_token_type::close_parentheses)
				: advance_if_unchecked(roy::detail::signature_parser::lexer::is_identifier_char)
					? make_identifier()
				: make_error(roy::detail::signature_parser::lexer_error_type::unexpected_character);
		}
	private:
		static constexpr auto is_whitespace(char c) noexcept
			-> bool{
			switch(c){
			case ' ': [[fallthrough]];
			case '\t': [[fallthrough]];
			case '\r': [[fallthrough]];
			case '\n':
				return true;
			default:
				return false;
			}
		}

		static constexpr auto is_identifier_char(char c) noexcept
			-> bool{
			if(c >= 'a' && c <= 'z')
				return true;

			if(c >= 'A' && c <= 'Z')
				return true;

			switch(c){
			case '_':
				return true;
			default:
				return false;
			}
		}

		static constexpr auto is_number_char(char c) noexcept
			-> bool{
			if(c >= '0' && c <= '9')
				return true;

			return false;
		}
	private:
		constexpr auto make_identifier() noexcept
			-> std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error>{
			while(peek(roy::detail::signature_parser::lexer::is_identifier_char).has_value()){
				m_current_index++;
			}

			std::string_view identifier{
				m_input.substr(
					m_token_start_index,
					m_current_index - m_token_start_index
				)
			};

			const roy::detail::signature_parser::lexer_token_type type{
				identifier.size() == 0
					? roy::detail::signature_parser::lexer_token_type::identifier
					: identifier == "__cdecl"
						? roy::detail::signature_parser::lexer_token_type::keyword_cdecl
					: identifier == "constexpr"
						? roy::detail::signature_parser::lexer_token_type::keyword_constexpr
					: identifier == "const"
						? roy::detail::signature_parser::lexer_token_type::keyword_const
					: identifier == "auto"
						? roy::detail::signature_parser::lexer_token_type::keyword_auto
					: identifier == "noexcept"
						? roy::detail::signature_parser::lexer_token_type::keyword_noexcept
					: identifier == "class"
						? roy::detail::signature_parser::lexer_token_type::keyword_class
					: identifier == "struct"
						? roy::detail::signature_parser::lexer_token_type::keyword_struct
					: identifier == "false"
						? roy::detail::signature_parser::lexer_token_type::keyword_false
					: identifier == "true"
						? roy::detail::signature_parser::lexer_token_type::keyword_true
					: identifier == "nullptr"
						? roy::detail::signature_parser::lexer_token_type::keyword_nullptr
					: identifier == "with"
						? roy::detail::signature_parser::lexer_token_type::keyword_with
					: roy::detail::signature_parser::lexer_token_type::identifier
			};

			return std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error>{
				std::in_place,
				m_input,
				m_token_start_index,
				m_current_index - m_token_start_index,
				type
			};
		}

		constexpr auto make_token(roy::detail::signature_parser::lexer_token_type type) const noexcept
			-> std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error>{
			return std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error>{
				std::in_place,
				m_input,
				m_token_start_index,
				m_current_index - m_token_start_index,
				type
			};
		}

		constexpr auto make_error(roy::detail::signature_parser::lexer_error_type type) const noexcept
			-> std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error>{
			return std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error>{
				std::unexpect,
				m_input,
				m_token_start_index,
				type
			};
		}

		constexpr auto is_at_end() const noexcept
			-> bool{
			return m_current_index >= m_input.size();
		}

		constexpr auto peek() const noexcept
			-> std::optional<char>{
			if(is_at_end())
				return std::nullopt;

			return m_input[m_current_index];
		}
		constexpr auto peek(char c) const noexcept
			-> bool{
			if(is_at_end())
				return false;

			return m_input[m_current_index] == c;
		}
		template<typename FPredicate>
		constexpr auto peek(FPredicate&& f) const noexcept
			-> std::optional<char>{
			if(is_at_end())
				return std::nullopt;

			char c{m_input[m_current_index]};
			if(not f(c))
				return std::nullopt;

			return c;
		}

		constexpr auto advance() noexcept
			-> std::optional<char>{
			if(is_at_end())
				return std::nullopt;

			return m_input[m_current_index++];
		}
		constexpr auto advance(char c) noexcept
			-> bool{
			if(peek(c)){
				m_current_index++;
				return true;
			}

			return false;
		}
		template<typename FPredicate>
		constexpr auto advance(FPredicate&& f) noexcept
			-> std::optional<char>{
			if(std::optional<char> result{peek(std::forward<FPredicate>(f))}; result.has_value()){
				m_current_index++;
				return result;
			}

			return std::nullopt;
		}
		template<typename FPredicate>
		constexpr auto advance_if(FPredicate&& f) noexcept
			-> bool{
			if(peek(std::forward<FPredicate>(f)).has_value()){
				m_current_index++;
				return true;
			}

			return false;
		}

		constexpr auto advance_unchecked() noexcept
			-> char{
			return m_input[m_current_index++];
		}
		constexpr auto advance_unchecked(char c) noexcept
			-> bool{
			if(m_input[m_current_index] == c){
				m_current_index++;
				return true;
			}

			return false;
		}
		template<typename FPredicate>
		constexpr auto advance_unchecked(FPredicate&& f) noexcept
			-> std::optional<char>{
			if(char c{m_input[m_current_index]}; f(c)){
				m_current_index++;
				return c;
			}

			return std::nullopt;
		}
		template<typename FPredicate>
		constexpr auto advance_if_unchecked(FPredicate&& f) noexcept
			-> bool{
			if(char c{m_input[m_current_index]}; f(c)){
				m_current_index++;
				return true;
			}

			return false;
		}

		constexpr auto skip_whitespace() noexcept
			-> bool{
			if(not advance_if_unchecked(roy::detail::signature_parser::lexer::is_whitespace))
				return false;

			while(peek(roy::detail::signature_parser::lexer::is_whitespace).has_value()){
				m_current_index++;
			}

			return true;
		}
	private:
		std::size_t m_token_start_index{0};
		std::size_t m_current_index{0};
		std::string_view m_input;
	};

	enum struct parser_error_type{
		expected_identifier,
		missing_closing_angled_bracket,
		missing_closing_parentheses,
		missing_closing_brackets,
		expected_template_arg,
		expected_opening_parentheses,
		expected_opening_brackets,
		expected_equals
	};
}

namespace roy::detail{
	inline auto to_string(roy::detail::signature_parser::parser_error_type type)
		-> std::string{
		switch(type){
		case roy::detail::signature_parser::parser_error_type::missing_closing_parentheses:
			return "missing ')'";
		case roy::detail::signature_parser::parser_error_type::missing_closing_brackets:
			return "missing ']'";
		case roy::detail::signature_parser::parser_error_type::missing_closing_angled_bracket:
			return "missing '>'";
		case roy::detail::signature_parser::parser_error_type::expected_identifier:
			return "expected identifier";
		case roy::detail::signature_parser::parser_error_type::expected_template_arg:
			return "expected template arg";
		case roy::detail::signature_parser::parser_error_type::expected_opening_parentheses:
			return "expected '('";
		case roy::detail::signature_parser::parser_error_type::expected_opening_brackets:
			return "expected '['";
		case roy::detail::signature_parser::parser_error_type::expected_equals:
			return "expected '='";
		default:
			return "";
		}
	}
}

namespace roy::detail::signature_parser{
	struct parser_error{
		std::string_view input;
		std::size_t position;
		roy::detail::signature_parser::parser_error_type type;
	};
}

namespace roy::detail{
	inline auto to_string(roy::detail::signature_parser::parser_error error)
		-> std::string{
		return std::format(
			"error: {} at (position #{} - line {}):\n{}\n{}",
			roy::detail::to_string(error.type),
			error.position + 1,
			static_cast<std::size_t>(std::floor(static_cast<double>(error.position + 1) / roy::detail::signature_parser::error_width)) + 1,
			error.input | std::views::chunk(roy::detail::signature_parser::error_width) | std::views::join_with('\n') | std::ranges::to<std::string>(),
			std::string(error.position % roy::detail::signature_parser::error_width, ' ') + '^'
		);
	}

	inline auto to_string(std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error> error)
		-> std::string{
		if(std::holds_alternative<roy::detail::signature_parser::lexer_error>(error))
			return roy::detail::to_string(std::get<roy::detail::signature_parser::lexer_error>(error));

		return roy::detail::to_string(std::get<roy::detail::signature_parser::parser_error>(error));
	}

	inline auto to_string(std::expected<std::string_view, std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>> result)
		-> std::string{
		if(result.has_value())
			return std::string{result.value()};

		return roy::detail::to_string(result.error());
	}
}

namespace roy::detail::signature_parser{
	// TODO: This whole parser is overcomplicated and of course isn't complete (especially the msvc part, which is mostly untested)
	// Probably not really possible to diambiguate between namespace names, type names and function names correctly.
	// Plus looking into the functions template for msvc to get a types/fields name is a pain.
	// of course, parsing C++ is... well, complicated. But parsing the source_location only should be easier
	// but I don't want to just use just std::string.find() or std::views::drop_while() etc., which seems kinda error-prone.
	class parser{
	public:
		static constexpr auto type_name(std::string_view input)
			-> std::expected<
			std::string_view,
			std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
		>{
			auto scan_result{roy::detail::signature_parser::parser::scan_all_tokens(input)};
			if(not scan_result.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					scan_result.error()
				};

			roy::detail::signature_parser::parser p{input, scan_result.value()};
			if(auto return_type_name{p.parse_most_inner_type_name()}; not return_type_name.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					return_type_name.error()
				};


#if defined(__clang__) || defined(__GNUC__) || defined(__GUNG__)
			auto function_name{p.parse_function_name()};
			if(not function_name.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					function_name.error()
				};

			auto params{p.parse_parameter_list()};
			if(not params.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					params.error()
				};

			auto template_assignments{p.parse_template_assignments()};
			if(not template_assignments.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					template_assignments.error()
				};

			if(template_assignments.value().size() < 1)
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					input,
					0,
					roy::detail::signature_parser::parser_error_type::expected_template_arg
				};

			return template_assignments.value().at(0).second;
#elif defined(_MSC_VER)
			auto template_arg_names{p.parse_most_inner_type_template_arg_names()};
			if(not template_arg_names.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					template_arg_names.error()
				};

			if(template_arg_names.value().size() < 1)
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					input,
					0,
					roy::detail::signature_parser::parser_error_type::expected_template_arg
				};

			return template_arg_names.value().at(0);
#endif
		}

		// atm same as type_name but might have to differ for msvc, not tested yet.
		static constexpr auto member_field_name(std::string_view input)
			-> std::expected<
			std::string_view,
			std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
		>{
			auto scan_result{roy::detail::signature_parser::parser::scan_all_tokens(input)};
			if(not scan_result.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					scan_result.error()
				};

			roy::detail::signature_parser::parser p{input, scan_result.value()};
			if(auto return_type_name{p.parse_most_inner_type_name()}; not return_type_name.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					return_type_name.error()
				};


#if defined(__clang__) || defined(__GNUC__) || defined(__GUNG__)
			auto function_name{p.parse_function_name()};
			if(not function_name.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					function_name.error()
				};

			auto params{p.parse_parameter_list()};
			if(not params.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					params.error()
				};

			auto template_assignments{p.parse_template_assignments()};
			if(not template_assignments.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					template_assignments.error()
				};

			if(template_assignments.value().size() < 1)
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					input,
					0,
					roy::detail::signature_parser::parser_error_type::expected_template_arg
				};

			return template_assignments.value().at(0).second;
#elif defined(_MSC_VER)
			auto template_arg_names{p.parse_most_inner_type_template_arg_names()};
			if(not template_arg_names.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					template_arg_names.error()
				};

			if(template_arg_names.value().size() < 1)
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					input,
					0,
					roy::detail::signature_parser::parser_error_type::expected_template_arg
				};

			return template_arg_names.value().at(0);
#endif
		}

		// atm same as type_name but might have to differ for msvc, not tested yet.
		static constexpr auto member_function_name(std::string_view input)
			-> std::expected<
			std::string_view,
			std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
		>{
			auto scan_result{roy::detail::signature_parser::parser::scan_all_tokens(input)};
			if(not scan_result.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					scan_result.error()
				};

			roy::detail::signature_parser::parser p{input, scan_result.value()};
			if(auto return_type_name{p.parse_most_inner_type_name()}; not return_type_name.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					return_type_name.error()
				};


#if defined(__clang__) || defined(__GNUC__) || defined(__GUNG__)
			auto function_name{p.parse_function_name()};
			if(not function_name.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					function_name.error()
				};

			auto params{p.parse_parameter_list()};
			if(not params.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					params.error()
				};

			auto template_assignments{p.parse_template_assignments()};
			if(not template_assignments.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					template_assignments.error()
				};

			if(template_assignments.value().size() < 1)
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					input,
					0,
					roy::detail::signature_parser::parser_error_type::expected_template_arg
				};

			return template_assignments.value().at(0).second;
#elif defined(_MSC_VER)
			auto template_arg_names{p.parse_most_inner_type_template_arg_names()};
			if(not template_arg_names.has_value())
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					template_arg_names.error()
				};

			if(template_arg_names.value().size() < 1)
				return std::expected<
					std::string_view,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::parser_error>,
					input,
					0,
					roy::detail::signature_parser::parser_error_type::expected_template_arg
				};

			return template_arg_names.value().at(0);
#endif
		}
	private:
		static constexpr auto take_pair_first(std::pair<std::string_view, std::vector<std::string_view>> p) noexcept
			-> std::string_view{
			return p.first;
		}
		static constexpr auto take_pair_second(std::pair<std::string_view, std::vector<std::string_view>> p) noexcept
			-> std::vector<std::string_view>{
			return p.second;
		}

		static constexpr auto make_pair_add_vector(std::string_view v) noexcept
			-> std::pair<std::string_view, std::vector<std::string_view>>{
			return std::pair<std::string_view, std::vector<std::string_view>>{
				v,
				std::vector<std::string_view>{}
			};
		}

		static constexpr auto has_ignored_token_type(roy::detail::signature_parser::lexer_token token) noexcept
			-> bool{
			switch(token.type){
			case roy::detail::signature_parser::lexer_token_type::keyword_cdecl: [[fallthrough]];
			case roy::detail::signature_parser::lexer_token_type::keyword_struct: [[fallthrough]];
			case roy::detail::signature_parser::lexer_token_type::keyword_class: [[fallthrough]];
			case roy::detail::signature_parser::lexer_token_type::keyword_constexpr: [[fallthrough]];
			case roy::detail::signature_parser::lexer_token_type::keyword_noexcept: [[fallthrough]];
			case roy::detail::signature_parser::lexer_token_type::keyword_const: [[fallthrough]];
			case roy::detail::signature_parser::lexer_token_type::double_ampersand:;
				return true;
			default:
				return false;
			}
		}
		static constexpr auto scan_all_tokens(std::string_view input)
			-> std::expected<
			std::vector<roy::detail::signature_parser::lexer_token>,
			std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
		>{
			roy::detail::signature_parser::lexer lexer{input};

			std::vector<roy::detail::signature_parser::lexer_token> tokens{};
			std::expected<roy::detail::signature_parser::lexer_token, roy::detail::signature_parser::lexer_error> current{
				lexer.next_token()
			};
			for(; current.has_value() && current.value().type != roy::detail::signature_parser::lexer_token_type::eof; current = lexer.next_token()){
				if(not roy::detail::signature_parser::parser::has_ignored_token_type(current.value()))
					tokens.push_back(current.value());
			}

			if(not current.has_value())
				return std::expected<
					std::vector<roy::detail::signature_parser::lexer_token>,
					std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
				>{
					std::unexpect,
					std::in_place_type<roy::detail::signature_parser::lexer_error>,
					current.error()
				};
			else
				tokens.push_back(current.value());

			return std::expected<
				std::vector<roy::detail::signature_parser::lexer_token>,
				std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>
			>{
				std::in_place,
				tokens
			};
		}

		constexpr parser(std::string_view input, std::span<roy::detail::signature_parser::lexer_token> tokens)
			: m_input{input}, m_tokens{tokens}{

		}

		constexpr auto parse_parameter_list() noexcept
			-> std::expected<std::vector<std::pair<std::string_view, std::string_view>>, roy::detail::signature_parser::parser_error>{
			if(auto opening_paren{match(roy::detail::signature_parser::lexer_token_type::open_parentheses)}; not opening_paren.has_value())
				return make_error<std::vector<std::pair<std::string_view, std::string_view>>>(
					roy::detail::signature_parser::parser_error_type::expected_opening_parentheses
				);

			std::vector<std::pair<std::string_view, std::string_view>> params{};
			if(auto closing_paren{match(roy::detail::signature_parser::lexer_token_type::close_parentheses)}; closing_paren.has_value())
				return params;

			do{
				consume(roy::detail::signature_parser::lexer_token_type::double_colon);

				auto type_name = parse_most_inner_type_name().transform(take_pair_first);

				if(not type_name.has_value())
					return std::expected<std::vector<std::pair<std::string_view, std::string_view>>, roy::detail::signature_parser::parser_error>{
						std::unexpect,
						type_name.error()
					};
				consume(roy::detail::signature_parser::lexer_token_type::ampersand);

				auto identifier = match(roy::detail::signature_parser::lexer_token_type::identifier);
				if(identifier.has_value())
					params.push_back(std::pair{type_name.value(), roy::detail::signature_parser::lexeme(identifier.value())});
				else
					params.push_back(std::pair{type_name.value(), ""});
			}while(match(roy::detail::signature_parser::lexer_token_type::comma));

			if(auto closing_paren{match(roy::detail::signature_parser::lexer_token_type::close_parentheses)}; not closing_paren.has_value())
				return make_error<std::vector<std::pair<std::string_view, std::string_view>>>(
					roy::detail::signature_parser::parser_error_type::missing_closing_parentheses
				);

			return params;
		}

		constexpr auto parse_most_inner_type_template_arg_names() noexcept
			-> std::expected<std::vector<std::string_view>, roy::detail::signature_parser::parser_error>{
			std::expected<std::vector<std::string_view>, roy::detail::signature_parser::parser_error> identifier{};
			do{
				consume(roy::detail::signature_parser::lexer_token_type::double_colon);
				if(identifier = parse_type_name().transform(take_pair_second); not identifier.has_value())
					return identifier;
			}while(match(roy::detail::signature_parser::lexer_token_type::double_colon).has_value());

			return identifier;
		}

		constexpr auto parse_function_name() noexcept
			-> std::expected<std::string_view, roy::detail::signature_parser::parser_error>{
			do{
				consume(roy::detail::signature_parser::lexer_token_type::double_colon);
				if(
					peek(0) == roy::detail::signature_parser::lexer_token_type::identifier and
					peek(1) == roy::detail::signature_parser::lexer_token_type::open_parentheses
				)
					break;

				if(auto identifier = parse_type_name().transform(take_pair_first); not identifier.has_value())
					return identifier;
			}while(match(roy::detail::signature_parser::lexer_token_type::double_colon).has_value());

			if(auto identifier{match(roy::detail::signature_parser::lexer_token_type::identifier)}; identifier.has_value())
				return roy::detail::signature_parser::lexeme(identifier.value());

			return make_error<std::string_view>(roy::detail::signature_parser::parser_error_type::expected_identifier);
		}

		constexpr auto parse_most_inner_type_name() noexcept
			-> std::expected<std::pair<std::string_view, std::vector<std::string_view>>, roy::detail::signature_parser::parser_error>{
			std::expected<std::pair<std::string_view, std::vector<std::string_view>>, roy::detail::signature_parser::parser_error> identifier{};
			do{
				consume(roy::detail::signature_parser::lexer_token_type::double_colon);
				if(identifier = parse_type_name(); not identifier.has_value())
					return identifier;
			}while(match(roy::detail::signature_parser::lexer_token_type::double_colon).has_value());

			return identifier;
		}

		constexpr auto parse_template_assignments() noexcept
			-> std::expected<std::vector<std::pair<std::string_view, std::string_view>>, roy::detail::signature_parser::parser_error>{
			if(auto opening_brack{match(roy::detail::signature_parser::lexer_token_type::open_brackets)}; not opening_brack.has_value())
				return make_error<std::vector<std::pair<std::string_view, std::string_view>>>(
					roy::detail::signature_parser::parser_error_type::expected_opening_brackets
				);

			bool has_with{match(roy::detail::signature_parser::lexer_token_type::keyword_with).has_value()};

			std::vector<std::pair<std::string_view, std::string_view>> assignments{};
			do{
				std::expected<std::string_view, roy::detail::signature_parser::parser_error> name{};
				if(has_with)
					name = "with";

				if(
					peek(0) == roy::detail::signature_parser::lexer_token_type::identifier and
					peek(1) == roy::detail::signature_parser::lexer_token_type::equals
				){
					name = roy::detail::signature_parser::lexeme(match(roy::detail::signature_parser::lexer_token_type::identifier).value());
				} else if(has_with && peek(0) == roy::detail::signature_parser::lexer_token_type::equals){

				} else {
					if(auto type_name{parse_most_inner_type_name()}; not type_name.has_value()){
						if(not match(roy::detail::signature_parser::lexer_token_type::keyword_auto).has_value())
							return std::expected<std::vector<std::pair<std::string_view, std::string_view>>, roy::detail::signature_parser::parser_error>{
								std::unexpect,
								type_name.error()
							};
					}
					consume(roy::detail::signature_parser::lexer_token_type::ampersand);

					if(auto name_identifier{match(roy::detail::signature_parser::lexer_token_type::identifier)}; name_identifier.has_value()){
						name = roy::detail::signature_parser::lexeme(name_identifier.value());
					} else if(has_with){

					} else{
						return make_error<std::vector<std::pair<std::string_view, std::string_view>>>(
							roy::detail::signature_parser::parser_error_type::expected_identifier
						);
					}
				}

				if(auto equals{match(roy::detail::signature_parser::lexer_token_type::equals)}; not equals.has_value())
					return make_error<std::vector<std::pair<std::string_view, std::string_view>>>(
						roy::detail::signature_parser::parser_error_type::expected_equals
					);
				
				consume(roy::detail::signature_parser::lexer_token_type::ampersand);
				auto value{parse_most_inner_type_name()};
				if(not value.has_value())
					return std::expected<std::vector<std::pair<std::string_view, std::string_view>>, roy::detail::signature_parser::parser_error>{
						std::unexpect,
						value.error()
					};

				assignments.push_back(std::pair<std::string_view, std::string_view>{name.value(), value.value().first});
			}while(match(roy::detail::signature_parser::lexer_token_type::semicolon).has_value());

			if(auto closing_brack{match(roy::detail::signature_parser::lexer_token_type::close_brackets)}; not closing_brack.has_value())
				return make_error<std::vector<std::pair<std::string_view, std::string_view>>>(
					roy::detail::signature_parser::parser_error_type::missing_closing_brackets
				);

			return assignments;
		}

		constexpr auto parse_template_arg_name() noexcept
			-> std::expected<std::string_view, roy::detail::signature_parser::parser_error>{
			// TODO: need to parse non-type template parameters?
			consume(roy::detail::signature_parser::lexer_token_type::ampersand);
			return parse_most_inner_type_name().transform(take_pair_first);
		}

		constexpr auto parse_type_name() noexcept
			-> std::expected<std::pair<std::string_view, std::vector<std::string_view>>, roy::detail::signature_parser::parser_error>{
			std::optional<roy::detail::signature_parser::lexer_token> identifier{match(roy::detail::signature_parser::lexer_token_type::identifier)};
			if(not identifier.has_value())
				return make_error<std::pair<std::string_view, std::vector<std::string_view>>>(
					roy::detail::signature_parser::parser_error_type::expected_identifier
				);

			std::vector<std::string_view> args{};

			// templated type
			if(match(roy::detail::signature_parser::lexer_token_type::open_angled_brackets).has_value()){
				do{
					if(auto template_arg{parse_template_arg_name()}; template_arg.has_value()){
						args.push_back(template_arg.value());
						consume(roy::detail::signature_parser::lexer_token_type::ampersand);
					} else{
						return template_arg.transform(make_pair_add_vector);
					}
				}while(match(roy::detail::signature_parser::lexer_token_type::comma).has_value());

				if(not match(roy::detail::signature_parser::lexer_token_type::close_angled_brackets).has_value())
					return make_error<std::pair<std::string_view, std::vector<std::string_view>>>(
						roy::detail::signature_parser::parser_error_type::missing_closing_angled_bracket
					);
			// function type
			} else if(match(roy::detail::signature_parser::lexer_token_type::open_parentheses).has_value()){
				bool has_multiple{false};
				do{
					if(auto type_param{parse_most_inner_type_name()}; type_param.has_value()){
						args.push_back(type_param.value().first);
						consume(roy::detail::signature_parser::lexer_token_type::ampersand);

						has_multiple = true;
					} else if(has_multiple){ // comma but no value -> error
						return type_param;
					}
				}while(match(roy::detail::signature_parser::lexer_token_type::comma).has_value());

				if(not match(roy::detail::signature_parser::lexer_token_type::close_parentheses).has_value())
					return make_error<std::pair<std::string_view, std::vector<std::string_view>>>(
						roy::detail::signature_parser::parser_error_type::missing_closing_parentheses
					);
			}

			return std::expected<std::pair<std::string_view, std::vector<std::string_view>>, roy::detail::signature_parser::parser_error>{
				std::in_place,
				roy::detail::signature_parser::lexeme(identifier.value()),
				args
			};
		}
	private:
		template<typename T>
		constexpr auto make_error(roy::detail::signature_parser::parser_error_type type) noexcept
			-> std::expected<T, roy::detail::signature_parser::parser_error>{
			return std::expected<T, roy::detail::signature_parser::parser_error>{
				std::unexpect,
				m_input,
				m_tokens[m_current_index].position,
				type
			};
		}
	private:
		constexpr auto is_at_end() const noexcept
			-> bool{
			return m_current_index >= m_tokens.size();
		}

		constexpr auto peek() const noexcept
			-> std::optional<roy::detail::signature_parser::lexer_token_type>{
			if(is_at_end())
				return std::nullopt;

			return m_tokens[m_current_index].type;
		}

		constexpr auto peek(std::size_t lookahead) const noexcept
			-> std::optional<roy::detail::signature_parser::lexer_token_type>{
			if(is_at_end())
				return std::nullopt;

			return m_tokens[m_current_index + lookahead].type;
		}

		constexpr auto peek(roy::detail::signature_parser::lexer_token_type token_type) const noexcept
			-> bool{
			if(is_at_end())
				return false;

			return m_tokens[m_current_index].type == token_type;
		}

		constexpr auto consume(roy::detail::signature_parser::lexer_token_type token_type) noexcept
			-> void{
			if(is_at_end())
				return;

			if(m_tokens[m_current_index].type != token_type)
				return;

			m_current_index++;
		}

		constexpr auto match(roy::detail::signature_parser::lexer_token_type token_type) noexcept
			-> std::optional<roy::detail::signature_parser::lexer_token>{
			if(is_at_end())
				return std::nullopt;

			if(token_type == roy::detail::signature_parser::lexer_token_type::identifier and m_tokens[m_current_index].type == roy::detail::signature_parser::lexer_token_type::keyword_with)
				return m_tokens[m_current_index++];

			if(m_tokens[m_current_index].type != token_type)
				return std::nullopt;

			return m_tokens[m_current_index++];
		}
	private:
		std::size_t m_current_index{0};
		std::string_view m_input;
		std::span<roy::detail::signature_parser::lexer_token> m_tokens;
	};
}

namespace roy::detail{
	template<typename T>
	inline constexpr auto get_type_name_expected_string_view() noexcept
		-> std::expected<std::string_view, std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>>{
		// some of the builtin types wouldn't be parsed correctly
		// because their name would be something like "unsigned long long"
		// but the parser expects a single identifier per type
		if constexpr (std::is_same_v<T, void>)
			return "void";
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
			return "nullptr";
		else if constexpr (std::is_same_v<T, std::nullopt_t>)
			return "nullopt";
		else if constexpr (std::is_same_v<T, bool>)
			return "bool";
		else if constexpr (std::is_same_v<T, std::uint8_t>)
			return "uint8";
		else if constexpr (std::is_same_v<T, std::uint16_t>)
			return "uint16";
		else if constexpr (std::is_same_v<T, std::uint32_t>)
			return "uint32";
		else if constexpr (std::is_same_v<T, std::uint64_t>)
			return "uint64";
		else if constexpr (std::is_same_v<T, __uint128_t>)
			return "uint128";
		else if constexpr (std::is_same_v<T, std::int8_t>)
			return "int8";
		else if constexpr (std::is_same_v<T, std::int16_t>)
			return "int16";
		else if constexpr (std::is_same_v<T, std::int32_t>)
			return "int32";
		else if constexpr (std::is_same_v<T, std::int64_t>)
			return "int64";
		else if constexpr (std::is_same_v<T, __int128_t>)
			return "int128";
		else if constexpr (std::is_same_v<T, float>)
			return "float32";
		else if constexpr (std::is_same_v<T, double>)
			return "float64";
		else if constexpr (std::is_same_v<T, long double>)
			return "float128";
		else if constexpr (std::is_same_v<T, char>)
			return "char";
		else if constexpr (std::is_same_v<T, wchar_t>)
			return "wchar";
		else if constexpr (std::is_same_v<T, char8_t>)
			return "char8";
		else if constexpr (std::is_same_v<T, char16_t>)
			return "char16";
		else if constexpr (std::is_same_v<T, char32_t>)
			return "char32";
		else
			return roy::detail::signature_parser::parser::type_name(
				roy::detail::get_type_name_signature<T>()
			);
	}

	template<auto FieldPtr>
	inline constexpr auto get_field_name_expected_string_view() noexcept
		-> std::expected<std::string_view, std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>>{
		return roy::detail::signature_parser::parser::member_field_name(
			roy::detail::get_member_field_name_signature<FieldPtr>()
		);
	}

	template<auto FunctionPtr>
	inline constexpr auto get_function_name_expected_string_view() noexcept
		-> std::expected<std::string_view, std::variant<roy::detail::signature_parser::lexer_error, roy::detail::signature_parser::parser_error>>{
		return roy::detail::signature_parser::parser::member_function_name(
			roy::detail::get_member_function_name_signature<FunctionPtr>()
		);
	}

	template<typename T>
	inline constexpr auto get_type_name_string_view() noexcept
		-> std::string_view{
		static_assert(roy::detail::get_type_name_expected_string_view<T>().has_value(), "parsing type name failed");
		return roy::detail::get_type_name_expected_string_view<T>().value();
	}

	template<typename T>
	inline constexpr auto get_type_name_size() noexcept
		-> std::size_t{
		return roy::detail::get_type_name_string_view<T>().size();
	}

	template<auto FieldPtr>
	inline constexpr auto get_field_name_string_view() noexcept
		-> std::string_view{
		static_assert(roy::detail::get_field_name_expected_string_view<FieldPtr>().has_value(), "parsing field name failed");
		return roy::detail::get_field_name_expected_string_view<FieldPtr>().value();
	}

	template<auto FieldPtr>
	inline constexpr auto get_field_name_size() noexcept
		-> std::size_t{
		return roy::detail::get_field_name_string_view<FieldPtr>().size();
	}

	template<auto FunctionPtr>
	inline constexpr auto get_function_name_string_view() noexcept
		-> std::string_view{
		static_assert(roy::detail::get_function_name_expected_string_view<FunctionPtr>().has_value(), "parsing function name failed");
		return roy::detail::get_function_name_expected_string_view<FunctionPtr>().value();
	}

	template<auto FunctionPtr>
	inline constexpr auto get_function_name_size() noexcept
		-> std::size_t{
		return roy::detail::get_function_name_string_view<FunctionPtr>().size();
	}
}

namespace roy::util{
	template<typename T>
	inline constexpr auto get_type_name() noexcept
		-> roy::util::inplace_string<roy::detail::get_type_name_size<T>() + 1>{
		return {
			[]<std::size_t... Is>(std::index_sequence<Is...>){
				return std::array<char, roy::detail::get_type_name_size<T>() + 1>{roy::detail::get_type_name_string_view<T>()[Is]..., '\0'};
			}(std::make_index_sequence<roy::detail::get_type_name_size<T>()>{})
		};
	}

	template<auto FieldPtr>
	inline constexpr auto get_field_name() noexcept
		-> roy::util::inplace_string<roy::detail::get_field_name_size<FieldPtr>() + 1>{
		return {
			[]<std::size_t... Is>(std::index_sequence<Is...>){
				return std::array<char, roy::detail::get_field_name_size<FieldPtr>() + 1>{roy::detail::get_field_name_string_view<FieldPtr>()[Is]..., '\0'};
			}(std::make_index_sequence<roy::detail::get_field_name_size<FieldPtr>()>{})
		};
	}

	template<auto FunctionPtr>
	inline constexpr auto get_function_name() noexcept
		-> roy::util::inplace_string<roy::detail::get_function_name_size<FunctionPtr>() + 1>{
		return {
			[]<std::size_t... Is>(std::index_sequence<Is...>){
				return std::array<char, roy::detail::get_function_name_size<FunctionPtr>() + 1>{roy::detail::get_function_name_string_view<FunctionPtr>()[Is]..., '\0'};
			}(std::make_index_sequence<roy::detail::get_function_name_size<FunctionPtr>() - 1>{})
		};
	}
}

namespace roy::detail{
	template<typename ForBuilder>
	struct get_name;

	template<typename Type, typename AvailableBuilderTags, typename ExtensionTags, typename Extensions>
	struct get_name<roy::detail::type_builder<Type, AvailableBuilderTags, ExtensionTags, Extensions>>{
		static constexpr auto value{roy::util::get_type_name<Type>()};
	};

	template<auto FieldPtr, typename AvailableBuilderTags, typename ExtensionTags, typename Extensions>
	struct get_name<roy::detail::field_builder<FieldPtr, AvailableBuilderTags, ExtensionTags, Extensions>>{
		static constexpr auto value{roy::util::get_field_name<FieldPtr>()};
	};

	template<typename ForBuilder>
	inline constexpr auto get_name_t{roy::detail::get_name<ForBuilder>::value};
}

namespace roy::util{
	template<typename ForBuilder>
	inline constexpr auto get_name() noexcept{
		return roy::detail::get_name_t<ForBuilder>;
	}
}

#endif
