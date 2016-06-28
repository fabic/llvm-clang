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


###
# cpp-netlib
#
# For the HTTP module. Defined here instead of within `http/CMakeLists.txt`
# so that we may eventually have other modules based on cpp-netlib.
#
# Defined here :
#   - ${LibCppNetlibClientConnections}
#   - ${LibCppNetlibServerParsers}
#
# FIXME: hard-coded paths, can't get my way 'round those CMake stuff...

  include_directories(${CMAKE_SOURCE_DIR}/../misc/cpp-netlib)
  include_directories(${CMAKE_SOURCE_DIR}/../misc/cpp-netlib/deps/uri/include)

  find_file(
    LibCppNetlibClientConnections
    libcppnetlib-client-connections.so
    PATHS ${CMAKE_SOURCE_DIR}/../misc/cpp-netlib/build/libs/network/src
    )

  find_file(
    LibCppNetlibServerParsers
    libcppnetlib-server-parsers.so
    PATHS ${CMAKE_SOURCE_DIR}/../misc/cpp-netlib/build/libs/network/src
    )
# ^ cpp-netlib
##


add_subdirectory(http)

