add_executable(demo.out)

target_sources(demo.out
	PRIVATE
		src/main.cxx
)

target_link_libraries(demo.out
	PRIVATE
		reflect-on-yourself::reflect-on-yourself
)

if(NOT MSVC)
	target_compile_options(demo.out
		PRIVATE
			-Wall
			-Wextra
			-pedantic
	)
endif()
