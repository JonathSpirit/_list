cmake_minimum_required(VERSION 3.2)
project(_list)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp C_list.inl C_list.hpp)
