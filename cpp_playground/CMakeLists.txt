cmake_minimum_required(VERSION 3.3)
project(cpp_playground)

include_directories(../misc/boost)
include_directories(../misc/boostcgi)

#link_directories(../misc/boost)
#set(PROJECT_LINK_LIBS boost_filesystem boost_system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(SOURCE_FILES main.cpp Process.cpp Process.h Syscall.h)
add_executable(cpp_playground ${SOURCE_FILES})


project('test1')
add_executable(test1 test1.cpp)
target_link_libraries(test1 boost_filesystem boost_system)


project('test2')
add_executable(test2 test2.cpp SomeClassA.cpp)


project('test_container')
add_executable(test_container test_container.cpp container.cpp SomeClassA.cpp SomeClassB.cpp)
target_link_libraries(test_container yaml-cpp boost_filesystem boost_system)


project('fcgi_echo')
add_executable(echo echo.cpp)
target_link_libraries(echo boost_filesystem boost_system)
install(TARGETS echo DESTINATION fcgi-bin)

project('mixed_datatype')
add_executable(mixed_main mixed_main.cpp)
