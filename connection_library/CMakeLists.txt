# connection library

file(GLOB SOURCES "*.c")
list(REMOVE_ITEM SOURCES "main.c")

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(elog_connection SHARED ${SOURCES})

add_executable(connection_tester main.c)
target_compile_options(connection_tester PRIVATE -Wall -ggdb)
set_target_properties(connection_tester PROPERTIES OUTPUT_NAME "connection_tester")
target_link_libraries(connection_tester elog_connection)
