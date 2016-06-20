# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
#
cmake_minimum_required(VERSION 3.3)

project(FabicCppPlayground)

set (FabicCppPlayground_VERSION_MAJOR 0)
set (FabicCppPlayground_VERSION_MINOR 1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++ -Wall")

# fixme: temp.
set(BOOST_ROOT ../local/boost-1.61.0-clang)
set(BOOST_INCLUDE_DIRS ${BOOST_ROOT}/include)
set(BOOST_LIBRARY_DIRS ${BOOST_ROOT}/local/boost-1.61.0-clang/lib)


find_package(Boost 1.61.0 REQUIRED)

if (Boost_FOUND)
	set (Boost_USE_MULTITHREAD ON)  # enable multithreading
	set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
	include_directories(${Boost_INCLUDE_DIR})
	find_package (Boost COMPONENTS REQUIRED program_options filesystem system)
endif()

#include_directories(../misc/boost)
#include_directories("${BOOT_ROOT}/include")
include_directories(../misc/boostcgi)

# find_library(YamlCpp yaml-cpp.so PATHS ../misc/yaml-cpp/build)
find_file(YamlCppLibSoPath libyaml-cpp.so PATHS ../misc/yaml-cpp/build)

set(SOURCE_FILES main.cpp Process.cpp Process.h Syscall.h)
add_executable(cpp_playground ${SOURCE_FILES})


# project('test1')
add_executable(test1 test1.cpp)
target_link_libraries(test1 boost_filesystem boost_system)


# project('test2')
add_executable(test2 test2.cpp SomeClassA.cpp)


# project('test_container')
add_executable(test_container test_container.cpp container.cpp SomeClassA.cpp SomeClassB.cpp hack_cxa_throw_exception.cpp)
target_include_directories(test_container PUBLIC ../../misc/yaml-cpp/include)
target_link_libraries(test_container ${YamlCppLibSoPath} ${Boost_LIBRARIES} unwind dl)


# project('fcgi_echo')
add_executable(echo echo.cpp)
target_link_libraries(echo boost_filesystem boost_system)
install(TARGETS echo DESTINATION fcgi-bin)


# project('mixed_datatype')
add_executable(mixed_main mixed_main.cpp)


# project('boost_playground')
add_executable(boost_options_debug_encoding boost_options_debug_encoding.cpp)
target_link_libraries(boost_options_debug_encoding ${Boost_LIBRARIES} dl unwind)
