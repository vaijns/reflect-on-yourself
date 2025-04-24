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
