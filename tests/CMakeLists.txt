# gtest lib directory
link_directories(lib)

# all test targets needs to be present here
add_custom_target(all_tests ALL DEPENDS select_query_test msg_parser_test)

# command invoking tests after build
add_custom_command(TARGET all_tests POST_BUILD COMMAND GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --output-on-failure)

# tests:

add_executable(msg_parser_test src/msg_parser_test.cpp)
target_link_libraries(msg_parser_test gtest e_log_lib pthread)
add_test(NAME msg_parser_test COMMAND msg_parser_test)

add_executable(select_query_test src/select_query_test.cpp)
target_link_libraries(select_query_test gtest e_log_lib pthread)
add_test(NAME select_query_test COMMAND select_query_test)
