cmake_minimum_required(VERSION 3.10)
project(_list)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master)

FetchContent_MakeAvailable(matplotplusplus)

#set flag -O1 in release mode
set(CMAKE_CXX_FLAGS_RELEASE "-O1 -DNDEBUG -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")

message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(${PROJECT_NAME} main.cpp C_list.inl)
target_sources(${PROJECT_NAME} PRIVATE FILE_SET HEADERS FILES C_list.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC matplot)
