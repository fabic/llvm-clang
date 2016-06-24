# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
#
cmake_minimum_required(VERSION 3.3)

project(FabicCppPlayground)

# This will cause -I. and similar/redondant directives like -I../symfony/..
# to be emitted.
# set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (FabicCppPlayground_VERSION_MAJOR 0)
set (FabicCppPlayground_VERSION_MINOR 1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++ -Wall")

###
# Boost C++
#
# https://cmake.org/cmake/help/latest/module/FindBoost.html
# https://svn.boost.org/trac/boost/wiki/CMakeBuildFeatures

  # Hard-coded path if needed for `find_package(Boost...)`
  #   - or simply set this as an environment variable,
  #   - or pass `-DBOOSTROOT=...` when running CMake.
  # (works ok with my CMake v3.4.3)
  set(BOOSTROOT ../local/boost-1.61.0-clang)

  # This one is for JetBrains' CLion bundled CMake (v3.5.2) :
  set(BOOST_ROOT ${BOOSTROOT})

  # These two ain't needed, actually...
  # set(BOOST_INCLUDE_DIRS ${BOOSTROOT}/include)
  # set(BOOST_LIBRARY_DIRS ${BOOSTROOT}/local/boost-1.61.0-clang/lib)

  #set (Boost_DEBUG)
  # ^ this turns out debug output _only for the CMake module!_.

  set (Boost_USE_MULTITHREAD ON)  # enable multithreading
  set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking

  add_definitions(-DBOOST_ALL_DYN_LINK)
  # ^ solves linking problems, notably with Boost.log library.
  #   See http://www.boost.org/doc/libs/1_61_0/libs/log/doc/html/log/rationale/namespace_mangling.html
  #   See also `symfony/include/fabic/logging.hpp`
  # ^ Note that the following would also solve the "unresolved symbol `v2s_mt_posix`"
  #   problem _for the `log` module only.
  #add_definitions(-DBOOST_LOG_DYN_LINK)

  find_package(Boost 1.61.0 REQUIRED)

  if (Boost_FOUND)
  	include_directories(${Boost_INCLUDE_DIR})
  	find_package (
      Boost
      COMPONENTS REQUIRED
        log
        #log_setup
        # ^ ain't needed it appears.
        program_options
        filesystem
        system
    )
  endif()

# ^ Boost C++
###


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
