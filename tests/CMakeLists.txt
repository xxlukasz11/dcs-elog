#unit tests

add_executable(tests
	src/unit_tests.cpp
	../src/msg_parser.cpp
)
target_link_libraries(tests gtest pthread)