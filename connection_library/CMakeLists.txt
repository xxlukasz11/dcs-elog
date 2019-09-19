# connection library

file(GLOB SOURCES "*.c")
list(REMOVE_ITEM SOURCES "main.c")

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(elog_connection SHARED ${SOURCES})

add_executable(tester main.c)
target_compile_options(tester PRIVATE -Wall -ggdb)
set_target_properties(tester PROPERTIES OUTPUT_NAME "tester")
target_link_libraries(tester elog_connection)
