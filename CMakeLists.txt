cmake_minimum_required(VERSION 3.24)
project(_list)

set(CMAKE_CXX_STANDARD 17)

add_executable(_list main.cpp C_list.inl C_list.hpp)
