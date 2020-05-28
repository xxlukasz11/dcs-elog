add_subdirectory(googletest)

# all test targets needs to be present here
add_custom_target(all_tests ALL DEPENDS select_query_test msg_parser_test file_name_parser_test utils_test base64_encoder_test concurrent_queue_test)

# command invoking tests after build
add_custom_command(TARGET all_tests POST_BUILD COMMAND GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --output-on-failure)

# libraries to link for tests
set(TEST_LINK_LIBRARIES gtest_main e_log_lib ${SQLITE_LIBRARY} pthread dl)

# tests:

add_executable(msg_parser_test src/msg_parser_test.cpp)
target_link_libraries(msg_parser_test ${TEST_LINK_LIBRARIES})
add_test(NAME msg_parser_test COMMAND msg_parser_test)

add_executable(select_query_test src/select_query_test.cpp)
target_link_libraries(select_query_test ${TEST_LINK_LIBRARIES})
add_test(NAME select_query_test COMMAND select_query_test)

add_executable(file_name_parser_test src/file_name_parser_test.cpp)
target_link_libraries(file_name_parser_test ${TEST_LINK_LIBRARIES})
add_test(NAME file_name_parser_test COMMAND file_name_parser_test)

add_executable(utils_test src/utils_test.cpp)
target_link_libraries(utils_test ${TEST_LINK_LIBRARIES})
add_test(NAME utils_test COMMAND utils_test)

add_executable(base64_encoder_test src/base64_encoder_test.cpp)
target_link_libraries(base64_encoder_test ${TEST_LINK_LIBRARIES})
add_test(NAME base64_encoder_test COMMAND base64_encoder_test)

add_executable(concurrent_queue_test src/concurrent_queue_test.cpp)
target_link_libraries(concurrent_queue_test ${TEST_LINK_LIBRARIES})
add_test(NAME concurrent_queue_test COMMAND concurrent_queue_test)
