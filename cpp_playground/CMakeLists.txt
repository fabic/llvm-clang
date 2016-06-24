# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
#
cmake_minimum_required(VERSION 3.3)

project(FabicCppPlayground)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (FabicCppPlayground_VERSION_MAJOR 0)
set (FabicCppPlayground_VERSION_MINOR 1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++ -Wall")

# fixme: temp.
set(BOOSTROOT ../local/boost-1.61.0-clang)
# set(BOOST_ROOT ../local/boost-1.61.0-clang)
# set(BOOST_INCLUDE_DIRS ${BOOST_ROOT}/include)
# set(BOOST_LIBRARY_DIRS ${BOOST_ROOT}/local/boost-1.61.0-clang/lib)


# find_package(Boost 1.61.0 REQUIRED)
find_package(Boost 1.56.0 REQUIRED)

if (Boost_FOUND)
	set (Boost_USE_MULTITHREAD ON)  # enable multithreading
	set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
	include_directories(${Boost_INCLUDE_DIR})
	find_package (Boost COMPONENTS REQUIRED program_options filesystem system)
endif()


add_subdirectory(symfony)


#include_directories(../misc/boost)
#include_directories("${BOOT_ROOT}/include")
#include_directories(../misc/boostcgi)

#find_library(YamlCpp yaml-cpp.so PATHS ../misc/yaml-cpp/build)
# ^ fixme: fails.
# find_library(YamlCpp yaml-cpp.so PATHS ../misc/yaml-cpp/build)

##
## cpp-netlib
##
## fixme: hard-coded paths, can't get my way 'round those CMake stuff...
##
# include_directories(../misc/cpp-netlib)
# include_directories(../misc/cpp-netlib/deps/uri/include)

# find_file(
# 	LibCppNetlibClientConnections
# 	libcppnetlib-client-connections.so
# 	PATHS ../misc/cpp-netlib/build/libs/network/src
# 	)

# find_file(
# 	LibCppNetlibServerParsers
# 	libcppnetlib-server-parsers.so
# 	PATHS ../misc/cpp-netlib/build/libs/network/src
# 	)

# set(SOURCE_FILES main.cpp Process.cpp Process.h Syscall.h)
# add_executable(cpp_playground ${SOURCE_FILES})


# # project('test1')
# add_executable(test1 test1.cpp)
# target_link_libraries(test1 boost_filesystem boost_system)


# # project('test2')
# add_executable(test2 test2.cpp SomeClassA.cpp)


# # project('test_container')
# add_executable(test_container test_container.cpp container.cpp SomeClassA.cpp SomeClassB.cpp hack_cxa_throw_exception.cpp)
# target_include_directories(test_container PUBLIC ../../misc/yaml-cpp/include)
# target_link_libraries(
# 	test_container
# 	${YamlCppLibSoPath}
# 	${Boost_LIBRARIES}
# 	unwind dl # for hack_cxa_throw_exception.cpp
# 	pthread
# 	)


# # project('fcgi_echo')
# add_executable(echo echo.cpp)
# target_link_libraries(echo boost_filesystem boost_system)
# install(TARGETS echo DESTINATION fcgi-bin)


# # project('mixed_datatype')
# add_executable(mixed_main mixed_main.cpp)


# # project('boost_playground')
# add_executable(boost_options_debug_encoding boost_options_debug_encoding.cpp)
# target_link_libraries(boost_options_debug_encoding ${Boost_LIBRARIES} dl unwind)


# add_executable(cppnetlib_test1 cppnetlib_test1.cpp hack_cxa_throw_exception.cpp)
# target_link_libraries(cppnetlib_test1
# 	${Boost_LIBRARIES}
# 	${LibCppNetlibClientConnections}
# 	${LibCppNetlibServerParsers}
# 	dl unwind
# 	pthread
# 	)


# add_executable(symfony symfony.cpp container.cpp hack_cxa_throw_exception.cpp)
# target_include_directories(test_container PUBLIC ../../misc/yaml-cpp/include)
# target_link_libraries(
# 	symfony
# 	${YamlCppLibSoPath}
# 	${Boost_LIBRARIES}
# 	${LibCppNetlibClientConnections}
# 	${LibCppNetlibServerParsers}
# 	unwind dl # for hack_cxa_throw_exception.cpp
# 	pthread
# 	)
