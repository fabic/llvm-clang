# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
#
cmake_minimum_required(VERSION 3.3)

project(Symfony)

# i.e. -Isymfony/include
include_directories(include)


###
# BINARY: build/symfony/symfony

  add_executable(
    symfony
    src/symfony.cpp
    src/di/container.cpp
    src/di/dll_service_provider.cpp
    src/hacks/hack_cxa_throw_exception.cpp
  )

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
    unwind dl # for hack_cxa_throw_exception.cpp
    dl        # for Boost.DLL
    pthread
    )

# ^ Symfony
###


###
# “ Modules ” ~~> libmodule-xxx.so
  add_subdirectory(module)
#
###
