cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(reflect-on-yourself
	VERSION 0.0.1
	DESCRIPTION "C++ reflection but you have to do it yourself"
	LANGUAGES CXX
)

add_library(reflect-on-yourself)
add_library(reflect-on-yourself::reflect-on-yourself ALIAS reflect-on-yourself)

target_sources(reflect-on-yourself
	PUBLIC
		FILE_SET reflect_on_yourself_headers
		TYPE HEADERS
		BASE_DIRS ./include
		FILES
			include/reflect-on-yourself.hpp

			include/reflect-on-yourself/reflection.hpp

			include/reflect-on-yourself/util/inplace_string.hpp
			include/reflect-on-yourself/util/type_wrapper.hpp
			include/reflect-on-yourself/util/field_ptr_declaring_type.hpp
			include/reflect-on-yourself/util/field_ptr_type.hpp
			include/reflect-on-yourself/util/get_name.hpp

			include/reflect-on-yourself/builder/builder.hpp
			include/reflect-on-yourself/builder/type_builder.hpp
			include/reflect-on-yourself/builder/field_builder.hpp
			include/reflect-on-yourself/builder/extended_builder.hpp

			include/reflect-on-yourself/extensions/name.hpp
			include/reflect-on-yourself/extensions/identifier.hpp
			include/reflect-on-yourself/extensions/fields.hpp
			include/reflect-on-yourself/extensions/annotation.hpp
			include/reflect-on-yourself/extensions/builders.hpp
			include/reflect-on-yourself/extensions/default_builders.hpp

			include/reflect-on-yourself/builtin_reflections/builtin_type_extension.hpp
			include/reflect-on-yourself/builtin_reflections/bool_type.hpp
			include/reflect-on-yourself/builtin_reflections/integer_types.hpp
			include/reflect-on-yourself/builtin_reflections/floating_point_types.hpp
	PRIVATE
		src/reflect-on-yourself.cxx
)

target_compile_features(reflect-on-yourself
	PUBLIC
		cxx_std_23
)

set_target_properties(reflect-on-yourself PROPERTIES
	CXX_EXTENSIONS OFF
)

install(
	TARGETS reflect-on-yourself
	EXPORT reflect-on-yourself-targets
	FILE_SET reflect_on_yourself_headers
)

install(
	EXPORT reflect-on-yourself-targets
	DESTINATION share/cmake/reflect-on-yourself
	NAMESPACE reflect-on-yourself::
)

install(
	FILES
		./cmake/reflect-on-yourself-config.cmake
	DESTINATION
		share/cmake/reflect-on-yourself
)

add_subdirectory(test)
add_subdirectory(examples)
