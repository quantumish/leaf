cmake_minimum_required(VERSION 3.14)
project(leaf LANGUAGES CXX)

set(PROJECT_NAME
    leaf
)

# set_target_properties(src PROPERTIES LINKER_LANGUAGE CXX)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TEST_DIR
    tests_cpp
)

set(SOURCE_DIR
    src
)

set(SOURCE
    ${SOURCE_DIR}/main.cpp
)

project(${PROJECT_NAME})

# ########
# GTest
# ########
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e49c6b946a44f9a58586bfc76f75687d8f77e725.zip
)

# # For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# ##########
# Project
# ##########
# add_executable(${PROJECT_NAME} ${SOURCE})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/${SOURCE_DIR}/constants.hpp
    ${CMAKE_SOURCE_DIR}/${SOURCE_DIR}/flametree.hpp
    ${CMAKE_SOURCE_DIR}/${SOURCE_DIR}/leaffunction.hpp
    ${CMAKE_SOURCE_DIR}/${SOURCE_DIR}/profiler.hpp
    ${CMAKE_SOURCE_DIR}/${SOURCE_DIR}/rapl.hpp
    ${CMAKE_SOURCE_DIR}/${SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} "${SOURCE_FILES}")


# #########################
# TESTING
# #########################
enable_testing()

set(TEST_FILES
    ${TEST_DIR}/test.cpp
)

add_executable(
    profiler_test
    "${TEST_FILES}"
)

target_link_libraries(
    profiler_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(profiler_test)

