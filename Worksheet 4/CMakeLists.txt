cmake_minimum_required(VERSION 3.10)

project(WS4)

set(CMAKE_CXX_STANDARD 17)

add_library(util util.cpp)

add_executable(hello hello.cpp)

target_link_libraries(hello util)
