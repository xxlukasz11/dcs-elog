#unit tests

add_executable(msg_parser_test
	src/msg_parser_test.cpp
	../src/msg_parser.cpp
	../src/variadic_exception.cpp
)
target_link_libraries(msg_parser_test gtest pthread)

add_executable(select_query_test
	src/select_query_test.cpp
	../src/select_query.cpp
	../src/prepared_statement.cpp
)
target_link_libraries(select_query_test gtest pthread)