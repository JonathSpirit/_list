cmake_minimum_required(VERSION 3.10)
project(_list)

set(CMAKE_CXX_STANDARD 17)

#set flag -O1 in release mode
set(CMAKE_CXX_FLAGS_RELEASE "-O1 -DNDEBUG")

message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(${PROJECT_NAME} main.cpp C_list.inl C_list.hpp)
