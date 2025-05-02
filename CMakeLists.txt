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
include(GNUInstallDirs)

set(REFLECT_ON_YOURSELF_INC_DIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE STRING "include directory")
set(REFLECT_ON_YOURSELF_LIB_DIR ${CMAKE_INSTALL_LIBDIR} CACHE STRING "lib directory")
set(REFLECT_ON_YOURSELF_BIN_DIR ${CMAKE_INSTALL_BINDIR} CACHE STRING "bin directory")
set(REFLECT_ON_YOURSELF_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/reflect-on-yourself CACHE STRING "cmake directory")
set(REFLECT_ON_YOURSELF_PKGCONFIG_DIR ${CMAKE_INSTALL_LIBDIR}/pkgconfig CACHE STRING "pkgconfig directory")
set(REFLECT_ON_YOURSELF_VERSION_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/reflect-on-yourself-version.cmake CACHE STRING "version config")
set(REFLECT_ON_YOURSELF_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/reflect-on-yourself-config.cmake CACHE STRING "config")

write_basic_package_version_file(
	${REFLECT_ON_YOURSELF_VERSION_CONFIG}
	VERSION ${PACKAGE_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	cmake/reflect-on-yourself-config.cmake.in
	${REFLECT_ON_YOURSELF_CONFIG}
	INSTALL_DESTINATION ${REFLECT_ON_YOURSELF_CMAKE_DIR}
)

install(
	TARGETS
		reflect-on-yourself
	EXPORT
		reflect-on-yourself-targets
	LIBRARY DESTINATION
		${REFLECT_ON_YOURSELF_LIB_DIR}
	ARCHIVE DESTINATION
		${REFLECT_ON_YOURSELF_LIB_DIR}
	INCLUDES DESTINATION
		${REFLECT_ON_YOURSELF_INC_DIR}
	RUNTIME DESTINATION
		${REFLECT_ON_YOURSELF_BIN_DIR}
	FILE_SET
		reflect_on_yourself_headers
)

install(
	EXPORT
		reflect-on-yourself-targets
	FILE
		reflect-on-yourself.cmake
	NAMESPACE
		reflect-on-yourself::
	DESTINATION
		${REFLECT_ON_YOURSELF_CMAKE_DIR}
)

install(
	FILES
		${REFLECT_ON_YOURSELF_CONFIG}
		${REFLECT_ON_YOURSELF_VERSION_CONFIG}
	DESTINATION
		${REFLECT_ON_YOURSELF_CMAKE_DIR}
)

add_subdirectory(test)
add_subdirectory(examples)
