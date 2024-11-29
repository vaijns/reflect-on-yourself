#ifndef H_REFLECT_ON_YOURSELF_TYPE_INFO
#define H_REFLECT_ON_YOURSELF_TYPE_INFO
#include "./field.hpp"
#include "./util.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdfloat>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace roy {
	template<auto Searched, auto Check, std::size_t Index>
	static consteval bool check() {
		using from = std::remove_cvref_t<decltype(Check)>;
		using to = std::remove_cvref_t<decltype(Searched)>;

		if constexpr(!std::is_convertible_v<from, to>)
			return false;
		else
			return static_cast<decltype(Searched)>(Check) == Searched;
	}

	template<auto Searched, auto... Rest, std::size_t... Is>
	static consteval std::size_t find_index_of(std::index_sequence<Is...>) {
		static_assert((check<Searched, Rest, Is>() || ...));

		std::size_t index{0};
		(
			[&]() {
				if constexpr(check<Searched, Rest, Is>())
					index = Is;
			}(),
			...
		);

		return index;
	}

	template<typename T, util::basic_string_literal Name, typename UFields = util::type_wrapper<>, typename VFunctions = util::type_wrapper<>, typename WExtensions = util::type_wrapper<>>
	struct type_info;

	template<typename T, util::basic_string_literal Name, typename... UFields, typename... VFunctions, typename... WExtensions>
	struct type_info<T, Name, util::type_wrapper<UFields...>, util::type_wrapper<VFunctions...>, util::type_wrapper<WExtensions...>> {
		static_assert((field_type_of<UFields, T> && ...), "All field pointers must be members of the type_info type T");

		using type_info_type =
			type_info<T, Name, util::type_wrapper<UFields...>, util::type_wrapper<VFunctions...>, util::type_wrapper<WExtensions...>>;

		using type = T;
		static constexpr auto name = Name;

		struct extensions : WExtensions::type... {
			using tag_list = util::type_wrapper<typename WExtensions::tag...>;
		};

		struct fields {
			using list = util::type_wrapper<UFields...>;

			static constexpr std::size_t size() { return sizeof...(UFields); }

			template<std::size_t N>
				requires(N < sizeof...(UFields))
			using nth_type = util::nth_type_of_t<N, util::type_wrapper<UFields...>>;

			template<std::size_t Index, util::basic_string_literal FieldAliasName>
				requires(Index < sizeof...(UFields))
			using alias_nth = type_info<
				T,
				Name,
				typename util::replace_nth_type_t<Index, util::type_wrapper<UFields...>, typename nth_type<Index>::template alias<FieldAliasName>>,
				typename util::type_wrapper<VFunctions...>,
				typename util::type_wrapper<WExtensions...>>;

			template<auto FieldPtr>
				requires(util::member_field_pointer_of<FieldPtr, T>)
			static constexpr std::size_t index_of =
				find_index_of<FieldPtr, UFields::pointer...>(std::make_index_sequence<sizeof...(UFields)>{});

			template<auto FieldPtr, util::basic_string_literal FieldAliasName>
				requires(util::member_field_pointer_of<FieldPtr, T>)
			using alias = type_info_type::fields::
				template alias_nth<type_info_type::fields::template index_of<FieldPtr>, FieldAliasName>;

			template<typename TExtensionTag>
			static constexpr std::size_t size_with_extension() {
				return util::
					type_count_v<util::filter_missing_tag_type_t<TExtensionTag, util::type_wrapper<UFields...>>>;
			}

			template<typename TExtensionTag, std::size_t N>
				requires(N < sizeof...(UFields))
			static constexpr bool nth_has_extension = nth_type<N>::template has_extension<TExtensionTag>;

			template<typename TExtensionTag, auto FieldPtr>
				requires(util::member_field_pointer_of<FieldPtr, T>)
			static constexpr bool has_extension =
				nth_type<index_of<FieldPtr>>::template has_extension<TExtensionTag>;

			template<typename TExtensionTag, std::size_t N>
			using nth_type_with_extension = util::
				nth_type_of_t<N, util::filter_missing_tag_type_t<TExtensionTag, util::type_wrapper<UFields...>>>;
		};

		struct functions {
			using list = util::type_wrapper<VFunctions...>;

			static constexpr std::size_t size() { return sizeof...(VFunctions); }

			template<std::size_t N>
				requires(N < sizeof...(VFunctions))
			using nth_type = util::nth_type_of_t<N, util::type_wrapper<VFunctions...>>;

			template<std::size_t Index, util::basic_string_literal FunctionAliasName>
				requires(Index < sizeof...(VFunctions))
			using alias_nth = type_info<
				T,
				Name,
				typename util::type_wrapper<UFields...>,
				typename util::replace_nth_type_t<Index, util::type_wrapper<VFunctions...>, typename nth_type<Index>::template alias<FunctionAliasName>>,
				typename util::type_wrapper<WExtensions...>>;

			template<auto FunctionPtr>
				requires(util::member_function_pointer_of<FunctionPtr, T>)
			static constexpr std::size_t index_of = find_index_of<FunctionPtr, VFunctions::pointer...>(
				std::make_index_sequence<sizeof...(VFunctions)>{}
			);

			template<auto FunctionPtr, util::basic_string_literal FunctionAliasName>
				requires(util::member_function_pointer_of<FunctionPtr, T>)
			using alias =
				type_info_type::functions::template alias_nth<index_of<FunctionPtr>, FunctionAliasName>;

			template<typename TExtensionTag>
			static constexpr std::size_t size_with_extension() {
				return util::
					type_count_v<util::filter_missing_tag_type_t<TExtensionTag, util::type_wrapper<VFunctions...>>>;
			}

			template<typename TExtensionTag, std::size_t N>
				requires(N < sizeof...(VFunctions))
			static constexpr bool nth_has_extension = nth_type<N>::template has_extension<TExtensionTag>;

			template<typename TExtensionTag, auto FunctionPtr>
				requires(util::member_function_pointer_of<FunctionPtr, T>)
			static constexpr bool has_extension =
				nth_type<index_of<FunctionPtr>>::template has_extension<TExtensionTag>;

			template<typename TExtensionTag, std::size_t N>
			using nth_type_with_extension = util::
				nth_type_of_t<N, util::filter_missing_tag_type_t<TExtensionTag, util::type_wrapper<VFunctions...>>>;
		};

		template<typename TExtension>
		using extend =
			type_info<T, Name, util::type_wrapper<UFields...>, util::type_wrapper<VFunctions...>, util::type_wrapper<WExtensions..., TExtension>>;

		template<util::basic_string_literal AliasName>
		using alias =
			type_info<T, AliasName, util::type_wrapper<UFields...>, util::type_wrapper<VFunctions...>, util::type_wrapper<WExtensions...>>;

		template<typename TExtensionTag>
		static constexpr bool has_extension = util::tag_type<type_info_type::extensions, TExtensionTag>;
	};

	template<
		typename T,
		util::basic_string_literal Name,
		template<typename...>
		typename UWrapper,
		typename... UFields,
		template<typename...>
		typename VWrapper,
		typename... VFunctions,
		template<typename...>
		typename WWrapper,
		typename... WExtensions>
	struct type_info<T, Name, UWrapper<UFields...>, VWrapper<VFunctions...>, WWrapper<WExtensions...>>
		: type_info<T, Name, util::type_wrapper<UFields...>, util::type_wrapper<VFunctions...>, util::type_wrapper<WExtensions...>> {
	};

	template<typename T>
	struct type_info_for;

	template<typename T>
	using type_info_for_t = type_info_for<T>::type;

	template<typename T>
	concept with_type_info = requires { typename type_info_for_t<T>; };

	template<typename T>
	struct is_type_info : std::false_type { };

	template<typename T, util::basic_string_literal Name, typename UFields, typename VFunctions, typename WExtensions>
	struct is_type_info<type_info<T, Name, UFields, VFunctions, WExtensions>> : std::true_type { };

	template<typename T>
	inline constexpr bool is_type_info_v = is_type_info<T>::value;

	template<typename T>
	concept type_info_type = is_type_info_v<T>;

	template<typename T, typename type_info>
	struct type_info_provider {
		using type = type_info;
	};

	template<typename T, util::basic_string_literal Name>
	struct fundamental_type_info {
		using type = T;
		static constexpr auto name = Name;

		struct fields {
			static constexpr std::size_t size() { return 0; }
		};

		struct functions {
			static constexpr std::size_t size() { return 0; }
		};
	};

	template<typename T, util::basic_string_literal Name>
	struct fundamental_type_info_provider {
		using type = fundamental_type_info<T, Name>;
	};

	template<>
	struct type_info_for<std::nullptr_t> : fundamental_type_info_provider<std::nullptr_t, "nullptr"> { };

	template<>
	struct type_info_for<bool> : fundamental_type_info_provider<bool, "bool"> { };

	template<>
	struct type_info_for<std::uint8_t> : fundamental_type_info_provider<std::uint8_t, "uint8"> { };

	template<>
	struct type_info_for<std::uint16_t> : fundamental_type_info_provider<std::uint16_t, "uint16"> { };

	template<>
	struct type_info_for<std::uint32_t> : fundamental_type_info_provider<std::uint32_t, "uint32"> { };

	template<>
	struct type_info_for<std::uint64_t> : fundamental_type_info_provider<std::uint64_t, "uint64"> { };
#ifdef __SIZEOF_UINT128__
	// template<> struct type_info_for<std::uint128_t>
	//: fundamental_type_info_provider<std::uint128_t, "uint128">{};
	// template<> struct type_info_for<__uint128_t>
	//: fundamental_type_info_provider<__uint128_t, "uint128">{};
	template<>
	struct type_info_for<unsigned __int128> : fundamental_type_info_provider<unsigned __int128, "uint128"> { };
#endif

	template<>
	struct type_info_for<std::int8_t> : fundamental_type_info_provider<std::int8_t, "int8"> { };

	template<>
	struct type_info_for<std::int16_t> : fundamental_type_info_provider<std::int16_t, "int16"> { };

	template<>
	struct type_info_for<std::int32_t> : fundamental_type_info_provider<std::int32_t, "int32"> { };

	template<>
	struct type_info_for<std::int64_t> : fundamental_type_info_provider<std::int64_t, "int64"> { };
#ifdef __SIZEOF_INT128__
	// template<> struct type_info_for<std::int128_t>
	//: fundamental_type_info_provider<std::int128_t, "int128">{};
	// template<> struct type_info_for<__int128_t>
	//: fundamental_type_info_provider<__int128_t, "int128">{};
	template<>
	struct type_info_for<__int128_t> : fundamental_type_info_provider<__int128_t, "int128"> { };
#endif

	// template<> struct type_info_for<unsigned char>
	//: fundamental_type_info_provider<unsigned char, "uchar">{};
	template<>
	struct type_info_for<char> : fundamental_type_info_provider<char, "char"> { };

	template<>
	struct type_info_for<wchar_t> : fundamental_type_info_provider<wchar_t, "wchar"> { };

	template<>
	struct type_info_for<char8_t> : fundamental_type_info_provider<char8_t, "char8"> { };

	template<>
	struct type_info_for<char16_t> : fundamental_type_info_provider<char16_t, "char16"> { };

	template<>
	struct type_info_for<char32_t> : fundamental_type_info_provider<char32_t, "char32"> { };

#ifdef __STDCPP_FLOAT_16_T__
	template<>
	struct type_info_for<std::float16_t> : fundamental_type_info_provider<std::float16_t, "float16_t"> { };
#endif
#ifdef __STDCPP_FLOAT_32_T__
	template<>
	struct type_info_for<std::float32_t> : fundamental_type_info_provider<std::float32_t, "float32_t"> { };
#else
	template<>
	struct type_info_for<float> : fundamental_type_info_provider<float, "float32_t"> { };
#endif
#ifdef __STDCPP_FLOAT_64_T__
	template<>
	struct type_info_for<std::float64_t> : fundamental_type_info_provider<std::float64_t, "float64_t"> { };
#else
	template<>
	struct type_info_for<double> : fundamental_type_info_provider<double, "float64_t"> { };
#endif
#ifdef __STDCPP_FLOAT_128_T__
	template<>
	struct type_info_for<std::float128_t> : fundamental_type_info_provider<std::float128_t, "float128_t"> { };
#else
	template<>
	struct type_info_for<long double> : fundamental_type_info_provider<long double, "float128_t"> { };
#endif
#ifdef __STDCPP_BFLOAT16_T__
	template<>
	struct type_info_for<std::bfloat16_t> : fundamental_type_info_provider<std::bfloat16_t, "bfloat16_t"> { };
#endif

	template<typename T>
	struct type_info_for<T*> : fundamental_type_info_provider<T*, "ptr<" + type_info_for_t<T>::name + ">"> { };

	template<>
	struct type_info_for<std::string> : fundamental_type_info_provider<std::string, "string"> { };

	template<>
	struct type_info_for<std::string_view> : fundamental_type_info_provider<std::string_view, "string"> { };

	template<typename T>
	struct type_info_for<std::optional<T>>
		: fundamental_type_info_provider<std::optional<T>, "optional<" + type_info_for_t<T>::name + ">"> { };

	template<typename T>
	struct type_info_for<std::vector<T>>
		: fundamental_type_info_provider<std::vector<T>, "vector<" + type_info_for_t<T>::name + ">"> { };

	template<typename T, std::size_t N>
	struct type_info_for<std::array<T, N>>
		: fundamental_type_info_provider<std::array<T, N>, "array<" + type_info_for_t<T>::name + ">"> { };
} // namespace roy
#endif
