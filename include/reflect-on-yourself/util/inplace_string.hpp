#ifndef H_REFLECT_ON_YOURSELF_UTIL_INPLACE_STRING
#define H_REFLECT_ON_YOURSELF_UTIL_INPLACE_STRING

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <format>

namespace roy::util{
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

		using it = std::basic_string<CharT, std::char_traits<CharT>>::iterator;

		struct iterator{
			using value_type = CharT;
			using element_type = CharT;
			using iterator_category = std::contiguous_iterator_tag;

			constexpr iterator()
				: m_sequence{nullptr}, m_index{0}{

			}
			constexpr iterator(CharT* sequence, std::size_t index)
				: m_sequence{sequence}, m_index{index}{

			}
			constexpr ~iterator() = default;
			constexpr iterator(const iterator&) = default;
			constexpr iterator(iterator&&) = default;
			constexpr auto operator=(const iterator&) -> iterator& = default;
			constexpr auto operator=(iterator&&) -> iterator& = default;

			constexpr auto operator->() const
				-> CharT*{
				return &m_sequence[m_index];
			}
			constexpr auto operator*() const
				-> CharT&{
				return m_sequence[m_index];
			}
			constexpr auto operator[](std::size_t index) const
				-> CharT&{
				return m_sequence[index];
			}
			constexpr auto operator++()
				-> iterator&{
				m_index++;
				return *this;
			}
			constexpr auto operator++(int)
				-> iterator{
				return iterator{m_sequence, m_index++};
			}
			constexpr auto operator--()
				-> iterator&{
				m_index--;
				return *this;
			}
			constexpr auto operator--(int)
				-> iterator{
				return iterator{m_sequence, m_index--};
			}
			constexpr auto operator+=(std::ptrdiff_t amount)
				-> iterator&{
				m_index += amount;
				return *this;
			}
			constexpr auto operator-=(std::ptrdiff_t amount)
				-> iterator&{
				m_index -= amount;
				return *this;
			}
			friend constexpr auto operator-(iterator lhs, iterator rhs)
				-> std::ptrdiff_t{
				return lhs.m_index - rhs.m_index;
			}
			friend constexpr auto operator+(iterator iter, std::ptrdiff_t amount)
				-> iterator{
				return iterator{iter.m_sequence, iter.m_index + amount};
			}
			friend constexpr auto operator-(iterator iter, std::ptrdiff_t amount)
				-> iterator{
				return iterator{iter.m_sequence, iter.m_index - amount};
			}
			friend constexpr auto operator+(std::ptrdiff_t amount, iterator iter)
				-> iterator{
				return iterator{iter.m_sequence, amount + iter.m_index};
			}
			friend constexpr auto operator-(std::ptrdiff_t amount, iterator iter)
				-> iterator{
				return iterator{iter.m_sequence, amount - iter.m_index};
			}
			friend constexpr auto operator<=>(iterator, iterator) = default;

			CharT* m_sequence;
			std::size_t m_index;
		};

		struct const_iterator{
			using value_type = CharT;
			using element_type = const CharT;
			using iterator_category = std::contiguous_iterator_tag;

			constexpr const_iterator()
				: m_sequence{nullptr}, m_index{0}{

			}
			constexpr const_iterator(const CharT* sequence, std::size_t index)
				: m_sequence{sequence}, m_index{index}{

			}
			constexpr ~const_iterator() = default;
			constexpr const_iterator(const const_iterator&) = default;
			constexpr const_iterator(const_iterator&&) = default;
			constexpr auto operator=(const const_iterator&) -> const_iterator& = default;
			constexpr auto operator=(const_iterator&&) -> const_iterator& = default;

			constexpr auto operator->() const
				-> const CharT*{
				return &m_sequence[m_index];
			}
			constexpr auto operator*() const
				-> const CharT&{
				return m_sequence[m_index];
			}
			constexpr auto operator[](std::size_t index) const
				-> const CharT&{
				return m_sequence[index];
			}
			constexpr auto operator++()
				-> const_iterator&{
				m_index++;
				return *this;
			}
			constexpr auto operator++(int)
				-> const_iterator{
				return const_iterator{m_sequence, m_index++};
			}
			constexpr auto operator--()
				-> const_iterator&{
				m_index--;
				return *this;
			}
			constexpr auto operator--(int)
				-> const_iterator{
				return const_iterator{m_sequence, m_index--};
			}
			constexpr auto operator+=(std::ptrdiff_t amount)
				-> const_iterator&{
				m_index += amount;
				return *this;
			}
			constexpr auto operator-=(std::ptrdiff_t amount)
				-> const_iterator&{
				m_index -= amount;
				return *this;
			}
			friend constexpr auto operator-(const_iterator lhs, const_iterator rhs)
				-> std::ptrdiff_t{
				return lhs.m_index - rhs.m_index;
			}
			friend constexpr auto operator+(const_iterator iter, std::ptrdiff_t amount)
				-> const_iterator{
				return const_iterator{iter.m_sequence, iter.m_index + amount};
			}
			friend constexpr auto operator-(const_iterator iter, std::ptrdiff_t amount)
				-> const_iterator{
				return const_iterator{iter.m_sequence, iter.m_index - amount};
			}
			friend constexpr auto operator+(std::ptrdiff_t amount, const_iterator iter)
				-> const_iterator{
				return const_iterator{iter.m_sequence, amount + iter.m_index};
			}
			friend constexpr auto operator-(std::ptrdiff_t amount, const_iterator iter)
				-> const_iterator{
				return const_iterator{iter.m_sequence, amount - iter.m_index};
			}
			friend constexpr auto operator<=>(const_iterator, const_iterator) = default;

			const CharT* m_sequence;
			std::size_t m_index;
		};

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static_assert(std::contiguous_iterator<iterator>);
		static_assert(std::contiguous_iterator<const_iterator>);
		static_assert(std::random_access_iterator<reverse_iterator>);
		static_assert(std::random_access_iterator<const_reverse_iterator>);

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
			return value[pos];
		}

		constexpr const_reference at(size_type pos) const {
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

		constexpr operator std::basic_string_view<value_type, traits_type>() const noexcept { return std::basic_string_view<value_type, traits_type>(value, length); }

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

		constexpr iterator begin() noexcept { return iterator{value, 0}; }

		constexpr iterator end() noexcept { return iterator{value, N - 1}; }

		constexpr const_iterator begin() const noexcept { return const_iterator{value, 0}; }

		constexpr const_iterator end() const noexcept { return const_iterator{value, N - 1}; }

		constexpr const_iterator cbegin() const noexcept { return const_iterator{value, 0}; }

		constexpr const_iterator cend() const noexcept { return const_iterator{value, N - 1}; }

		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{iterator{value, N - 1}}; }

		constexpr reverse_iterator rend() noexcept { return reverse_iterator{iterator{value, 0}}; }

		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{const_iterator{value, N - 1}}; }

		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{const_iterator{value, 0}}; }

		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{const_iterator{value, N - 1}}; }

		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{const_iterator{value, 0}}; }

		value_type value[N]{};
	};

	template<std::size_t N>
	using inplace_string = basic_inplace_string<char, N, std::char_traits<char>>;
	template<std::size_t N>
	using winplace_string = basic_inplace_string<wchar_t, N, std::char_traits<wchar_t>>;
	template<std::size_t N>
	using u8inplace_string = basic_inplace_string<char8_t, N, std::char_traits<char8_t>>;
	template<std::size_t N>
	using u16inplace_string = basic_inplace_string<char16_t, N, std::char_traits<char16_t>>;
	template<std::size_t N>
	using u32inplace_string = basic_inplace_string<char32_t, N, std::char_traits<char32_t>>;

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
}

template<typename CharT, std::size_t N, typename Traits>
struct std::formatter<roy::util::basic_inplace_string<CharT, N, Traits>>{
	constexpr formatter() = default;

	constexpr auto parse(std::format_parse_context& ctx){
		return ctx.begin();
	}

	constexpr auto format(const roy::util::basic_inplace_string<CharT, N, Traits>& str, std::format_context& ctx) const{
		return std::format_to(ctx.out(), "{}", str.value);
	}
};
#endif
