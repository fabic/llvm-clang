
cmake_minimum_required(VERSION 3.3)

project(Symf)

set (Symf_VERSION_MAJOR 0)
set (Symf_VERSION_MINOR 1)

add_definitions()


# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
# DEPENDENCIES
# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
find_package ( Boost
  COMPONENTS REQUIRED
    log
    program_options
    filesystem
    system
  )


# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
# TARGETS
# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

add_executable( symf
  symf_main.cpp
  ../di/container.cpp
  ../di/dll_service_provider.cpp
  ../hacks/hack_cxa_throw_exception.cpp
  # FIXME: Link modules along the main exec. due to pb with ABI :
  #dic-modules/http/module-http-server.cpp
  ../di/dic-modules/asio/module-boost-asio.cpp
  ../di/dic-modules/asio/IoService.cpp
  )

# For Boost.DLL which recommends this flag
#   http://www.boost.org/doc/libs/1_61_0/doc/html/boost_dll/getting_started.html
#   https://cmake.org/cmake/help/latest/command/target_compile_options.html
target_compile_options(
  symf
  PUBLIC
  )

target_link_libraries(
  symf
  ${Boost_LIBRARIES}
  #${LibCppNetlibClientConnections}
  #${LibCppNetlibServerParsers}
  unwind dl # for hack_cxa_throw_exception.cpp
  dl        # for Boost.DLL
  pthread
  )
