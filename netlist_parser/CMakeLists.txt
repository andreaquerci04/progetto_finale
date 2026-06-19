cmake_minimum_required(VERSION 3.20)

project(netlist_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Werror=vla"
)

add_executable(parser_demo
    parser_demo.cpp
)