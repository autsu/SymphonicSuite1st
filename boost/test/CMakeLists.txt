cmake_minimum_required(VERSION 3.19)
project(tests)

set(CMAKE_CXX_STANDARD 14)
set(gtest_SOURCE_DIR, ../../pkg/googletest/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(${PROJECT_NAME} test.cc)
target_link_libraries(tests gtest gtest_main)
