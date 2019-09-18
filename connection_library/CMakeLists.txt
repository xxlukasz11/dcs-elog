cmake_minimum_required(VERSION 2.8.9)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(GLOBAL PROPERTY C_STANDARD 11)

file(GLOB SOURCES "*.c")
list(REMOVE_ITEM SOURCES "main.c")

add_library(elog_connection SHARED ${SOURCES})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(tester main.c)
target_compile_options(tester PRIVATE -Wall -ggdb)
set_target_properties(tester PROPERTIES OUTPUT_NAME "tester")
target_link_libraries(tester elog_connection)
