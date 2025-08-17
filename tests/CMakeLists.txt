include(FetchContent)

function(simpleAddTest TARGET SOURCES DEPENDS)
    add_executable(${TARGET} ${SOURCES})
    target_link_libraries(${TARGET} PRIVATE ${DEPENDS} doctestMain)
    add_test(NAME ${TARGET} COMMAND ${TARGET} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()

set(DOCTEST_NO_INSTALL ON)
FetchContent_Declare(doctest
    GIT_REPOSITORY "https://github.com/doctest/doctest.git"
    GIT_TAG v2.4.12
)
FetchContent_MakeAvailable(doctest)

add_library(doctestMain STATIC doctestMain.cpp)
target_link_libraries(doctestMain PUBLIC doctest::doctest)

set(TESTS_DEPENDENCIES
    gg_list
)

simpleAddTest(basicListTests test_basic_list.cpp "${TESTS_DEPENDENCIES}")
