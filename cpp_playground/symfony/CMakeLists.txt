# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
#
cmake_minimum_required(VERSION 3.3)

project(Symfony)

include_directories(include)

###
# cpp-netlib
#
# fixme: hard-coded paths, can't get my way 'round those CMake stuff...

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


add_executable(
  symfony
  src/symfony.cpp
  src/di/container.cpp
  src/di/dll_service_provider.cpp
  src/hacks/hack_cxa_throw_exception.cpp)

# For Boost.DLL which recommends this flag
#   http://www.boost.org/doc/libs/1_61_0/doc/html/boost_dll/getting_started.html
#   https://cmake.org/cmake/help/latest/command/target_compile_options.html
target_compile_options(
  symfony
  PUBLIC
  -fvisibility=hidden
  )

target_link_libraries(
  symfony
  ${Boost_LIBRARIES}
  ${LibCppNetlibClientConnections}
  ${LibCppNetlibServerParsers}
  unwind dl # for hack_cxa_throw_exception.cpp
  dl        # for Boost.DLL
  pthread
  )
