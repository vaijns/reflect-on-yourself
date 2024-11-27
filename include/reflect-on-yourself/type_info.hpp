#ifndef H_REFLECT_ON_YOURSELF_TYPE_INFO
#define H_REFLECT_ON_YOURSELF_TYPE_INFO
#include <cstddef>
#include <cstdint>
#include <stdfloat>
#include <string>
#include <optional>
#include <vector>
#include <array>
#include <string_view>
#include <utility>
#include <type_traits>
#include "./util.hpp"
#include "./field.hpp"

namespace roy{
	enum struct type_info_flags{
		none = 0,
		is_fundamental = 1 << 0,
	};

	template<typename T, util::string_literal Name, typename... Fields>
	struct type_info{
		using type = T;
		static constexpr auto name = Name;

		struct fields{
			static constexpr std::size_t size(){
				return sizeof...(Fields);
			}
			template<std::size_t N>
			using nth_type = util::nth_type_of_t<N, Fields...>;
		};
		/*struct functions{
			constexpr std::size_t size(){
				return sizeof...(Functions);
			}
			template<std::size_t N>
			using nth_type = util::nth_type_of_t<N, Functions...>;
		};*/
	};

	template<typename T>
	struct type_info_for;

	template<typename T>
	using type_info_for_t = type_info_for<T>::type;

	template<typename T, typename type_info>
	struct type_info_provider{
		using type = type_info;
	};

	template<typename T, util::string_literal Name>
	struct fundamental_type_info{
		using type = T;
		static constexpr auto name = Name;

		struct fields{
			static constexpr std::size_t size(){
				return 0;
			}
		};
		static constexpr type_info_flags flags{
			type_info_flags::is_fundamental
		};
	};

	template<typename T, util::string_literal Name>
	struct fundamental_type_info_provider{
		using type = fundamental_type_info<T, Name>;
	};

	template<> struct type_info_for<std::nullptr_t>
		: fundamental_type_info_provider<std::nullptr_t, "nullptr">{};

	template<> struct type_info_for<bool>
		: fundamental_type_info_provider<bool, "bool">{};

	template<> struct type_info_for<std::uint8_t>
		: fundamental_type_info_provider<std::uint8_t, "uint8">{};
	template<> struct type_info_for<std::uint16_t>
		: fundamental_type_info_provider<std::uint16_t, "uint16">{};
	template<> struct type_info_for<std::uint32_t>
		: fundamental_type_info_provider<std::uint32_t, "uint32">{};
	template<> struct type_info_for<std::uint64_t>
		: fundamental_type_info_provider<std::uint64_t, "uint64">{};
	//template<> struct type_info_for<std::uint128_t>
		//: fundamental_type_info_provider<std::uint128_t, "uint128">{};
	//template<> struct type_info_for<__uint128_t>
		//: fundamental_type_info_provider<__uint128_t, "uint128">{};
	template<> struct type_info_for<unsigned __int128>
		: fundamental_type_info_provider<unsigned __int128, "uint128">{};

	template<> struct type_info_for<std::int8_t>
		: fundamental_type_info_provider<std::int8_t, "int8">{};
	template<> struct type_info_for<std::int16_t>
		: fundamental_type_info_provider<std::int16_t, "int16">{};
	template<> struct type_info_for<std::int32_t>
		: fundamental_type_info_provider<std::int32_t, "int32">{};
	template<> struct type_info_for<std::int64_t>
		: fundamental_type_info_provider<std::int64_t, "int64">{};
	//template<> struct type_info_for<std::int128_t>
		//: fundamental_type_info_provider<std::int128_t, "int128">{};
	//template<> struct type_info_for<__int128_t>
		//: fundamental_type_info_provider<__int128_t, "int128">{};
	template<> struct type_info_for<__int128_t>
		: fundamental_type_info_provider<__int128_t, "int128">{};

	//template<> struct type_info_for<unsigned char>
		//: fundamental_type_info_provider<unsigned char, "uchar">{};
	template<> struct type_info_for<char>
		: fundamental_type_info_provider<char, "char">{};
	template<> struct type_info_for<wchar_t>
		: fundamental_type_info_provider<wchar_t, "wchar">{};
	template<> struct type_info_for<char8_t>
		: fundamental_type_info_provider<char8_t, "char8">{};
	template<> struct type_info_for<char16_t>
		: fundamental_type_info_provider<char16_t, "char16">{};
	template<> struct type_info_for<char32_t>
		: fundamental_type_info_provider<char32_t, "char32">{};

#ifdef __STDCPP_FLOAT_16_T__
	template<> struct type_info_for<std::float16_t>
		: fundamental_type_info_provider<std::float16_t, "float16_t">{};
#endif
#ifdef __STDCPP_FLOAT_32_T__
	template<> struct type_info_for<std::float32_t>
		: fundamental_type_info_provider<std::float32_t, "float32_t">{};
#else
	template<> struct type_info_for<float>
		: fundamental_type_info_provider<float, "float32_t">{};
#endif
#ifdef __STDCPP_FLOAT_64_T__
	template<> struct type_info_for<std::float64_t>
		: fundamental_type_info_provider<std::float64_t, "float64_t">{};
#else
	template<> struct type_info_for<double>
		: fundamental_type_info_provider<double, "float64_t">{};
#endif
#ifdef __STDCPP_FLOAT_128_T__
	template<> struct type_info_for<std::float128_t>
		: fundamental_type_info_provider<std::float128_t, "float128_t">{};
#else
	template<> struct type_info_for<long double>
		: fundamental_type_info_provider<long double, "float128_t">{};
#endif
#ifdef __STDCPP_BFLOAT16_T__
	template<> struct type_info_for<std::bfloat16_t>
		: fundamental_type_info_provider<std::bfloat16_t, "bfloat16_t">{};
#endif

	template<typename T> struct type_info_for<T*>
		: fundamental_type_info_provider<T*, "ptr<" + type_info_for_t<T>::name + ">">{};

	template<> struct type_info_for<std::string>
		: fundamental_type_info_provider<std::string, "string">{};
	template<> struct type_info_for<std::string_view>
		: fundamental_type_info_provider<std::string_view, "string">{};

	template<typename T> struct type_info_for<std::optional<T>>
		: fundamental_type_info_provider<std::optional<T>, "optional<" + type_info_for_t<T>::name + ">">{};
	template<typename T> struct type_info_for<std::vector<T>>
		: fundamental_type_info_provider<std::vector<T>, "vector<" + type_info_for_t<T>::name + ">">{};
	template<typename T, std::size_t N> struct type_info_for<std::array<T, N>>
		: fundamental_type_info_provider<std::array<T, N>, "array<" + type_info_for_t<T>::name + ">">{};
}
#endif
