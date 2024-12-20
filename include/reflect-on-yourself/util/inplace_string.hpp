#ifndef H_REFLECT_ON_YOURSELF_UTIL_STRING_LITERAL
#define H_REFLECT_ON_YOURSELF_UTIL_STRING_LITERAL
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <type_traits>

namespace roy::util {
	template<typename CharT, std::size_t N, typename Traits = std::char_traits<CharT>>
	struct basic_inplace_string {
		using traits_type = Traits;
		using value_type = CharT;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = std::basic_string_view<value_type, traits_type>::iterator;
		using const_iterator = std::basic_string_view<value_type, traits_type>::const_iterator;
		using reverse_iterator = std::basic_string_view<value_type, traits_type>::reverse_iterator;
		using const_reverse_iterator = std::basic_string_view<value_type, traits_type>::const_reverse_iterator;

		static constexpr size_type length = N - 1;

		constexpr basic_inplace_string() noexcept { value[0] = '\0'; }

		constexpr basic_inplace_string(const CharT (&str)[N]) noexcept { std::copy_n(str, length, value); }

		constexpr basic_inplace_string(const basic_inplace_string&) = default;
		constexpr basic_inplace_string(basic_inplace_string&&) = default;

		constexpr basic_inplace_string& operator=(const basic_inplace_string&) = default;
		constexpr basic_inplace_string& operator=(basic_inplace_string&&) = default;

		constexpr basic_inplace_string(const std::array<CharT, N> str) noexcept {
			std::copy_n(str.data(), length, value);
		}

		constexpr reference at(size_type pos) {
			// if(not std::is_constant_evaluated() and pos >= length)
			// throw std::out_of_range(std::format("index {} is out of bounds of basic_inplace_string
			// with size {}.", pos, length));
			return value[pos];
		}

		constexpr const_reference at(size_type pos) const {
			// if(not std::is_constant_evaluated() and pos >= length)
			// throw std::out_of_range(std::format("index {} is out of bounds of basic_inplace_string
			// with size {}.", pos, length));
			return value[pos];
		}

		constexpr reference operator[](size_type pos) { return value[pos]; }

		constexpr const_reference operator[](size_type pos) const { return value[pos]; }

		constexpr reference front() { return value[0]; }

		constexpr const_reference front() const { return value[0]; }

		constexpr reference back() noexcept { return value[length - 1]; }

		constexpr const_reference back() const noexcept { return value[length - 1]; }

		constexpr pointer data() noexcept { return value; }

		constexpr const_pointer data() const noexcept { return value; }

		constexpr const_pointer c_str() const noexcept { return value; }

		constexpr operator std::basic_string_view<value_type, traits_type>() const noexcept { return value; }

		constexpr bool empty() const noexcept { return length == 0; }

		static constexpr size_type size() noexcept { return length; }

		constexpr size_type capacity() const noexcept { return length; }

		template<size_type Pos = 0, size_type Count = length>
		constexpr basic_inplace_string<value_type, Count + 1, traits_type> substr() const noexcept {
			std::array<char, Count + 1> substring{};
			std::copy_n(&value[Pos], Count, substring.data());
			return basic_inplace_string<value_type, Count + 1, traits_type>{substring};
		}

		constexpr std::basic_string_view<value_type, traits_type>
			substr(size_type pos, size_type count = length) const noexcept {
			return std::basic_string_view<value_type, traits_type>{&value[pos], count};
		}

		constexpr iterator begin() noexcept { return iterator{&value[0]}; }

		constexpr iterator end() noexcept { return iterator{&value[N - 1]}; }

		constexpr const_iterator begin() const noexcept { return const_iterator(&value[0]); }

		constexpr const_iterator end() const noexcept { return const_iterator(&value[N - 1]); }

		constexpr const_iterator cbegin() const noexcept { return const_iterator(&value[0]); }

		constexpr const_iterator cend() const noexcept { return const_iterator(&value[N - 1]); }

		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{&value[N - 1]}; }

		constexpr reverse_iterator rend() noexcept { return reverse_iterator{&value[0]}; }

		constexpr const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(&value[N - 1]);
		}

		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(&value[0]); }

		constexpr const_reverse_iterator crbegin() const noexcept {
			return const_reverse_iterator(&value[N - 1]);
		}

		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(&value[0]); }

		value_type value[N]{};
	};

	template<std::size_t N>
	using inplace_string = basic_inplace_string<char, N>;
	template<std::size_t N>
	using winplace_string = basic_inplace_string<wchar_t, N>;
	template<std::size_t N>
	using u8inplace_string = basic_inplace_string<char8_t, N>;
	template<std::size_t N>
	using u16inplace_string = basic_inplace_string<char16_t, N>;
	template<std::size_t N>
	using u32inplace_string = basic_inplace_string<char32_t, N>;

	template<std::size_t N, typename CharT, typename Traits = std::char_traits<CharT>>
	constexpr std::basic_ostream<CharT, Traits>&
		operator<<(std::basic_ostream<CharT, Traits>& stream, const basic_inplace_string<CharT, N, Traits>& str) {
		stream << str.value;
		return stream;
	}

	template<std::size_t NA, std::size_t NB, typename CharT, typename Traits = std::char_traits<CharT>>
	constexpr basic_inplace_string<CharT, NA + NB - 1, Traits>
		operator+(const basic_inplace_string<CharT, NA, Traits>& a, const basic_inplace_string<CharT, NB, Traits>& b) noexcept {
		std::array<CharT, NA + NB - 1> new_value{};
		std::copy_n(a.value, NA - 1, new_value.data());
		std::copy_n(b.value, NB, new_value.data() + NA - 1);

		return basic_inplace_string<CharT, NA + NB - 1, Traits>{new_value};
	}

	template<std::size_t NA, std::size_t NB, typename CharT, typename Traits = std::char_traits<CharT>>
	constexpr basic_inplace_string<CharT, NA + NB - 1, Traits>
		operator+(const CharT (&a)[NA], const basic_inplace_string<CharT, NB, Traits>& b) noexcept {
		std::array<CharT, NA + NB - 1> new_value{};
		std::copy_n(a, NA - 1, new_value.data());
		std::copy_n(b.value, NB, new_value.data() + NA - 1);

		return basic_inplace_string<CharT, NA + NB - 1, Traits>{new_value};
	}

	template<std::size_t NA, std::size_t NB, typename CharT, typename Traits = std::char_traits<CharT>>
	constexpr basic_inplace_string<CharT, NA + NB - 1, Traits>
		operator+(const basic_inplace_string<CharT, NA, Traits>& a, const CharT (&b)[NB]) noexcept {
		std::array<CharT, NA + NB - 1> new_value{};
		std::copy_n(a.value, NA - 1, new_value.data());
		std::copy_n(b, NB, new_value.data() + NA - 1);

		return basic_inplace_string<CharT, NA + NB - 1, Traits>{new_value};
	}

	template<std::size_t N, typename CharT, typename Traits = std::char_traits<CharT>>
	constexpr basic_inplace_string<CharT, N + 1, Traits>
		operator+(CharT ch, const basic_inplace_string<CharT, N, Traits>& b) noexcept {
		std::array<CharT, N + 1> new_value{};
		new_value[0] = ch;
		std::copy_n(b.value, N, new_value.data() + 1);

		return basic_inplace_string<CharT, N + 1, Traits>{new_value};
	}

	template<std::size_t N, typename CharT, typename Traits = std::char_traits<CharT>>
	constexpr basic_inplace_string<CharT, N + 1, Traits>
		operator+(const basic_inplace_string<CharT, N, Traits>& a, CharT ch) noexcept {
		std::array<CharT, N + 1> new_value{};
		std::copy_n(a.value, N - 1, new_value.data());
		new_value[N - 1] = ch;
		new_value[N] = a.value[N - 1];

		return basic_inplace_string<CharT, N + 1, Traits>{new_value};
	}

	template<std::size_t N, typename CharT>
	basic_inplace_string(const CharT (&str)[N]) -> basic_inplace_string<CharT, N, std::char_traits<CharT>>;
} // namespace roy::util
#endif
