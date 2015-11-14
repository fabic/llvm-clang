cmake_minimum_required(VERSION 3.3)
project(cpp_playground)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Process.cpp Process.h Syscall.h)
add_executable(cpp_playground ${SOURCE_FILES})