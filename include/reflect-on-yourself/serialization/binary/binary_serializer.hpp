#ifndef H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_SERIALIZER
#define H_REFLECT_ON_YOURSELF_SERIALIZATION_BINARY_SERIALIZER

#include <cstring>
#include <span>
#include <cstddef>
#include <array>
#include <optional>
#include <concepts>
#include <memory>
#include <limits>
#include <bitset>
#include <ranges>

#include "./serialization_buffer.hpp"
#include "./size_calculator.hpp"
#include "./data_type.hpp"
#include "../serialization_strategy.hpp"

namespace roy::serialization::detail{
	class binary_serializer{
	public:
		template<typename T>
		static constexpr auto serialize(const T& data)
			-> std::optional<roy::serialization::detail::serialization_read_buffer>{
			roy::serialization::detail::serialization_write_buffer buffer{roy::serialization::detail::size_calculator::calculate_size(data) + 1};
			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, data);
			roy::serialization::detail::binary_serializer::serialize_type_into(buffer, data);
			return roy::serialization::detail::serialization_read_buffer(std::move(buffer));
		}
	private:
		template<roy::serialization_strategy<roy::serialization::strategy::as_object> T>
			requires requires(T t) { roy::field_count_of<T>(); roy::reflection_of<T>::name(); }
		static constexpr auto serialize_into(roy::serialization::detail::serialization_write_buffer& buffer, const T& data)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, data);
			roy::serialization::detail::binary_serializer::serialize_type_into<T>(buffer);
		}
	private:
		template<roy::serialization_strategy<roy::serialization::strategy::as_array> T>
			requires std::ranges::forward_range<T>
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, const T& data)
			-> void{
			for(const auto& value : data){
				roy::serialization::detail::binary_serializer::serialize_value_into(buffer, value);
			}
			std::size_t size{std::size(data)};
			if(size > 0)
				roy::serialization::detail::binary_serializer::serialize_type_into(buffer, *std::begin(data));
			else
				roy::serialization::detail::binary_serializer::serialize_type_into(buffer, nullptr);
			roy::serialization::detail::binary_serializer::serialize_size_into(buffer, size);
		}
		template<roy::serialization_strategy<roy::serialization::strategy::as_object> T>
			requires requires(T t) { roy::field_count_of<T>(); roy::reflection_of<T>::name(); }
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, const T& data)
			-> void{
			if constexpr(roy::field_count_of<T>() > 0){
				[&]<std::size_t... Is>(std::index_sequence<Is...>){
					([&]<typename TProp, std::size_t N>(const TProp& prop, const roy::util::inplace_string<N>& name){
						roy::serialization::detail::binary_serializer::serialize_value_into(buffer, prop);
						roy::serialization::detail::binary_serializer::serialize_identifier_into(buffer, name);
						roy::serialization::detail::binary_serializer::serialize_type_into(buffer, prop);
					}(roy::nth_field_value<sizeof...(Is) - Is - 1>(data), roy::nth_field_name_of<sizeof...(Is) - Is - 1, T>()), ...);
				}(std::make_index_sequence<roy::field_count_of<T>()>{});
			}

			roy::serialization::detail::binary_serializer::serialize_size_into(buffer, roy::field_count_of<T>());
			roy::serialization::detail::binary_serializer::serialize_identifier_into(buffer, roy::reflection_of<T>::name());
		}
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, roy::serialization::detail::data_info data)
			-> void{
			roy::serialization::detail::binary_serializer::write_bytes_into(buffer, data);
		}
		template<typename E> requires std::is_enum_v<E>
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, E data)
			-> void{
			roy::serialization::detail::binary_serializer::write_bytes_into(buffer, static_cast<std::underlying_type_t<E>>(data));
		}
		template<std::integral I>
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, I data)
			-> void{
			roy::serialization::detail::binary_serializer::write_bytes_into(buffer, data);
		}
		template<std::floating_point F>
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, F data)
			-> void{
			roy::serialization::detail::binary_serializer::write_bytes_into(buffer, data);
		}
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, bool data)
			-> void{
			roy::serialization::detail::binary_serializer::write_bytes_into(buffer, data);
		}
		template<typename T>
		static constexpr auto serialize_value_into(roy::serialization::detail::serialization_write_buffer& buffer, const std::optional<T>& data)
			-> void{
			if(not data.has_value())
				return;

			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, data.value());
		}
		template<typename CharT>
		static constexpr auto serialize_value_into(
			roy::serialization::detail::serialization_write_buffer& buffer,
			const std::basic_string<CharT, std::char_traits<CharT>>& data
		) -> void{
			buffer.copy_bytes(data.data(), data.size() * sizeof(CharT));
			roy::serialization::detail::binary_serializer::serialize_size_into(buffer, data.size());
			buffer.add_string_capacity<CharT>(data.size());
		}
		template<typename CharT>
		static constexpr auto serialize_value_into(
			roy::serialization::detail::serialization_write_buffer& buffer,
			std::basic_string_view<CharT, std::char_traits<CharT>> data
		) -> void{
			buffer.copy_bytes(data.data(), data.size() * sizeof(CharT));
			roy::serialization::detail::binary_serializer::serialize_size_into(buffer, data.size());
			buffer.add_string_capacity<CharT>(data.size());
		}
		template<typename CharT>
		static constexpr auto serialize_value_into(
			roy::serialization::detail::serialization_write_buffer& buffer,
			const CharT* data
		) -> void{
			std::size_t size{roy::serialization::detail::size_calculator::null_terminated_string_length(data)};

			buffer.copy_bytes(data, size * sizeof(CharT));
			roy::serialization::detail::binary_serializer::serialize_size_into(buffer, size);
			buffer.add_string_capacity<CharT>(size);
		}
		template<typename CharT, std::size_t N>
		static constexpr auto serialize_string_literal_value_into(
			roy::serialization::detail::serialization_write_buffer& buffer,
			const CharT (&data)[N]
		) -> void{
			buffer.copy_bytes(data, (N - 1) * sizeof(CharT));
			roy::serialization::detail::binary_serializer::serialize_size_into(buffer, (N - 1));
			buffer.add_string_capacity<CharT>(N - 1);
		}
		template<typename CharT, std::size_t N>
		static constexpr auto serialize_value_into(
			roy::serialization::detail::serialization_write_buffer& buffer,
			const roy::util::basic_inplace_string<CharT, N, std::char_traits<CharT>>& data
		) -> void{
			buffer.copy_bytes(data.data(), (N - 1) * sizeof(CharT));
			roy::serialization::detail::binary_serializer::serialize_size_into(buffer, (N - 1));
			buffer.add_string_capacity<CharT>(N - 1);
		}
	private:
		template<typename T> requires requires(T t) { typename roy::serialization::detail::get_data_type<T>; }
		static constexpr auto serialize_type_into(roy::serialization::detail::serialization_write_buffer& buffer)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(
				buffer,
				roy::serialization::detail::data_info{
					.size_bytes{roy::serialization::detail::size_bytes::one_byte},
					.type{roy::serialization::detail::get_data_type_v<T>}
				}
			);
		}
		template<typename T> requires requires(T t) { typename roy::serialization::detail::get_data_type<T>; } and
			(not roy::serialization_strategy<T, roy::serialization::strategy::as_object>) and
			(not roy::serialization_strategy<T, roy::serialization::strategy::as_array>)
		static constexpr auto serialize_type_into(roy::serialization::detail::serialization_write_buffer& buffer, T)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_type_into<T>(buffer);
		}
		template<roy::serialization_strategy<roy::serialization::strategy::as_array> T>
			// TODO: non-object types like std::optional<T> could cause problems because there is only one data_type written per array,
			// not one per value but it can differ per value (nullopt or the value type).
			// So array values must be serialized differently than property values, so memory size can still be kept relatively small
			// (e.g. optional has an additional true/false value at the beginning for whether there is a value and in false case there is no value following)
			requires roy::serialization_strategy<std::ranges::range_value_t<T>, roy::serialization::strategy::as_object>
		static constexpr auto serialize_type_into(roy::serialization::detail::serialization_write_buffer& buffer, const T& data)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(
				buffer,
				roy::serialization::detail::data_info{
					.size_bytes{roy::serialization::detail::size_calculator::count_size_bytes(std::size(data))},
					.type{roy::serialization::detail::data_type::array}
				}
			);
		}

		template<roy::serialization_strategy<roy::serialization::strategy::as_object> T>
			requires requires(T t) { roy::field_count_of<T>(); roy::reflection_of<T>::name(); }
		static constexpr auto serialize_type_into(roy::serialization::detail::serialization_write_buffer& buffer, T)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(
				buffer,
				roy::serialization::detail::data_info{
					.size_bytes{roy::serialization::detail::size_calculator::count_size_bytes(roy::field_count_of<T>())},
					.type{roy::serialization::detail::data_type::object}
				}
			);
		}

		template<typename T>
		static constexpr auto serialize_type_into(roy::serialization::detail::serialization_write_buffer& buffer, const std::optional<T>& data)
			-> void{
			if(not data.has_value())
				roy::serialization::detail::binary_serializer::serialize_type_into<std::nullopt_t>(buffer);
			else
				roy::serialization::detail::binary_serializer::serialize_type_into(buffer, data.value());
		}
	private:
		static constexpr auto serialize_identifier_into(roy::serialization::detail::serialization_write_buffer& buffer, const char* identifier)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, identifier);
		}
		static constexpr auto serialize_identifier_into(roy::serialization::detail::serialization_write_buffer& buffer, const std::string& identifier)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, identifier);
		}
		static constexpr auto serialize_identifier_into(roy::serialization::detail::serialization_write_buffer& buffer, std::string_view identifier)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, identifier);
		}
		template<std::size_t N>
		static constexpr auto serialize_identifier_into(roy::serialization::detail::serialization_write_buffer& buffer, const char (&identifier)[N])
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, identifier);
		}
		template<std::size_t N>
		static constexpr auto serialize_identifier_into(roy::serialization::detail::serialization_write_buffer& buffer, const roy::util::inplace_string<N>& identifier)
			-> void{
			roy::serialization::detail::binary_serializer::serialize_value_into(buffer, identifier);
		}
	private:
		static constexpr auto serialize_size_into(roy::serialization::detail::serialization_write_buffer& buffer, std::size_t size)
			-> void{
			if(size <= roy::serialization::detail::size_calculator::max_bit_value((sizeof(std::uint8_t) * 8))){
				roy::serialization::detail::binary_serializer::write_bytes_into(buffer, static_cast<std::uint8_t>(size));
			} else if(size <= roy::serialization::detail::size_calculator::max_bit_value((sizeof(std::uint16_t) * 8) - 1)){
				roy::serialization::detail::binary_serializer::write_bytes_into(buffer, static_cast<std::uint16_t>(size));
			} else if(size <= roy::serialization::detail::size_calculator::max_bit_value((sizeof(std::uint32_t) * 8) - 2)){
				roy::serialization::detail::binary_serializer::write_bytes_into(buffer, static_cast<std::uint32_t>(size));
			} else if(size <= roy::serialization::detail::size_calculator::max_bit_value((sizeof(std::uint64_t) * 8) - 3)){
				roy::serialization::detail::binary_serializer::write_bytes_into(buffer, static_cast<std::uint64_t>(size));
			} else{
				throw;
			}
		}
	private:
		template<typename T>
		static constexpr auto write_bytes_into(roy::serialization::detail::serialization_write_buffer& buffer, T t)
			-> void{
			buffer.copy_bytes(&t, roy::serialization::detail::size_calculator::calculate_size<T>());
		}
	};
}

#endif
