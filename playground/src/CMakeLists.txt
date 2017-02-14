
cmake_minimum_required(VERSION 3.3)

# FYI: included from ../CMakeLists.txt

## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Symf / DIC experiments.
#
if (true)
  project(Symf)

  add_executable(
    symf
    symf/symf_main.cpp
    di/container.cpp
    di/dll_service_provider.cpp
    hacks/hack_cxa_throw_exception.cpp
    # FIXME: Link modules along the main exec. due to pb with ABI :
      dic-modules/http/module-http-server.cpp
      dic-modules/asio/module-boost-asio.cpp
      dic-modules/asio/IoService.cpp
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
    ${LibCppNetlibClientConnections}
    ${LibCppNetlibServerParsers}
    unwind dl # for hack_cxa_throw_exception.cpp
    dl        # for Boost.DLL
    pthread
    )

  # “ Modules ” ~~shared~libraries~~> libmodule-xxx.so
  add_subdirectory(dic-modules)

endif (true)
# ^ Symf
## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


add_subdirectory(clong)

add_subdirectory(surface)

add_subdirectory(syscall)
