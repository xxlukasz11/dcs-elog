#main app

file(GLOB SOURCES "*.cpp" "*.c")
add_executable(e_log ${SOURCES})

target_compile_options(e_log PRIVATE -Wall -ggdb)
set_target_properties(e_log PROPERTIES OUTPUT_NAME "e_log")
target_link_libraries(e_log sqlite3-shared pthread dl)