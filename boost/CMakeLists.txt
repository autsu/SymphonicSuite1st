cmake_minimum_required(VERSION 3.19)
project(boost)

set(CMAKE_CXX_STANDARD 14)
# 将指定目录添加到编译器的头文件搜索路径之下，指定的目录被解释成当前源码路径的相对路径
include_directories(/opt/homebrew/Cellar/boost/1.78.0_1/include)
include_directories(/opt/homebrew/Cellar/boost/1.78.0_1/lib)

add_executable(
        boost test_boost_include.cc
)