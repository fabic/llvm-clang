# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
#
cmake_minimum_required(VERSION 3.3)

project(Symfony-modules)


# i.e. -Isymfony/module/include
include_directories(include)


add_subdirectory(http)

