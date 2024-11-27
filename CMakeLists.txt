cmake_minimum_required(VERSION 3.31)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS False)

project(
	reflect-on-yourself
	VERSION 0.0.1
	DESCRIPTION ""
	HOMEPAGE_URL "https://localhost"
	LANGUAGES CXX
)

add_executable(${PROJECT_NAME}.out src/main.cpp)
target_include_directories(${PROJECT_NAME}.out PRIVATE include)
