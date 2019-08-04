#main app

file(GLOB SOURCES "*.cpp" "*.c")
list(REMOVE_ITEM SOURCES "main.cpp")

add_library(e_log_lib ${SOURCES})
add_executable(e_log main.cpp)

target_compile_options(e_log PRIVATE -Wall -ggdb)
set_target_properties(e_log PROPERTIES OUTPUT_NAME "e_log")
target_link_libraries(e_log e_log_lib ${SQLITE_LIBRARY} pthread dl)
