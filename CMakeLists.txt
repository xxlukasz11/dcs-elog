cmake_minimum_required(VERSION 2.8.9)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set_property(GLOBAL PROPERTY C_STANDARD 11)

enable_testing()
project (e_log_project)
include(CTest)

include_directories(include)
link_directories(lib)

add_subdirectory(src)
add_subdirectory(tests)