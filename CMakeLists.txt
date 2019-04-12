cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

project (e_log)

include_directories("${PROJECT_SOURCE_DIR}/include")
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(e_log ${SOURCES})

set_property(TARGET e_log PROPERTY CXX_STANDARD 14)
set_property(TARGET e_log PROPERTY C_STANDARD 11)

target_compile_options(e_log PRIVATE -Wall -ggdb)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "e_log")
set(CMAKE_EXE_LINKER_FLAGS "-L../lib -Wl,-rpath=../lib")
target_link_libraries(e_log sqlite3-shared pthread dl)