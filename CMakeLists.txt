cmake_minimum_required(VERSION 2.8.9)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set_property(GLOBAL PROPERTY C_STANDARD 11)

enable_testing()
project (e_log_project)
include(CTest)

include_directories(include)

set(SQLITE_LIBRARY sqlite3)
if(NOT DEFINED NOSQLITE)
	set(SQLITE_LIBRARY sqlite3-shared)
	add_subdirectory(sqlite)
	include_directories(sqlite)
	link_directories(sqlite/lib)
endif()
unset(NOSQLITE CACHE)

add_subdirectory(src)
add_subdirectory(connection_library)

if(NOT DEFINED NOTESTS)
	add_subdirectory(tests)
endif()
unset(NOTESTS CACHE)
