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
	LINKER_LANGUAGE CXX
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/reflect-on-yourself-config-version.cmake"
	VERSION ${PACKAGE_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	cmake/reflect-on-yourself-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/reflect-on-yourself-config.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/reflect-on-yourself/cmake"
)

install(
	TARGETS
		reflect-on-yourself
	EXPORT
		reflect-on-yourself-targets
	LIBRARY DESTINATION
		lib
	ARCHIVE DESTINATION
		lib
	RUNTIME DESTINATION
		bin
	INCLUDES DESTINATION
		include
	FILE_SET
		reflect_on_yourself_headers
	COMPONENT
		reflect-on-yourself
)

install(
	EXPORT
		reflect-on-yourself-targets
	FILE
		reflect-on-yourself.cmake
	NAMESPACE
		reflect-on-yourself::
	DESTINATION
		"${CMAKE_INSTALL_DATADIR}/reflect-on-yourself/cmake"
	COMPONENT
		reflect-on-yourself
)

install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/reflect-on-yourself-config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/reflect-on-yourself-config-version.cmake"
	DESTINATION
		"${CMAKE_INSTALL_DATADIR}/reflect-on-yourself/cmake"
	COMPONENT
		reflect-on-yourself
)

add_subdirectory(test)
add_subdirectory(examples)
