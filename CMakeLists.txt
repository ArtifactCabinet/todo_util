cmake_minimum_required(VERSION 3.17)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

find_package (SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(untitled main.cpp)
target_link_libraries (untitled ${SQLite3_LIBRARIES})

# DEBUG MODE
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(untitled PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(untitled PUBLIC DEBUG=0)
endif()