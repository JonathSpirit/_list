cmake_minimum_required(VERSION 3.10)
project(_list)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

#Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCH "Build benchmarks" ON)

#set flag -O1 in release mode
set(CMAKE_CXX_FLAGS_RELEASE "-O1 -DNDEBUG -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")

message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

add_library(gg_list INTERFACE)
target_sources(gg_list INTERFACE FILE_SET HEADERS FILES C_list.hpp C_list.inl)

if (BUILD_BENCH)
    FetchContent_Declare(matplotplusplus
            GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
            GIT_TAG origin/master)

    FetchContent_MakeAvailable(matplotplusplus)

    add_executable(listBench main.cpp)
    target_link_libraries(listBench PRIVATE gg_list matplot)
endif()

#Tests
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()