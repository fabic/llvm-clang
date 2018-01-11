
cmake_minimum_required(VERSION 3.3)

project(Symfony-module-http)

#
## NOTE: See parent CMakeLists.txt for cpp-netlib setup stuff.
#


###
# build/symfony/libmodule-http-server.so
if (false)
  # https://cmake.org/cmake/help/v3.0/command/add_library.html
  add_library(
    module-http-server
    MODULE
    module-http-server.cpp
    )

  target_link_libraries(
    module-http-server
    ${Boost_LIBRARIES}
    #${LibCppNetlibClientConnections}
    #${LibCppNetlibServerParsers}
    )
endif()
# ^ build/symfony/libmodule-http-server.so
###
