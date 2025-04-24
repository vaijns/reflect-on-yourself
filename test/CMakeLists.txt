add_executable(reflect-on-yourself.test)

target_sources(reflect-on-yourself.test
	PRIVATE
		src/main.cxx
)

target_link_libraries(reflect-on-yourself.test
	PRIVATE
		reflect-on-yourself::reflect-on-yourself
)
