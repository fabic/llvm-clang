
cmake_minimum_required(VERSION 3.3)

project(Syscalls)

add_executable( aaa
    tests/test001.cpp
    #../hacks/hack_cxa_throw_exception.cpp
    )

target_compile_options( sf
  PUBLIC
  # ...
)

target_include_directories( sf
  PUBLIC
  # ...
)

target_link_libraries( sf
  # ${Boost_LIBRARIES}
  # ${XCB_LIBRARIES}
  # ${XCB_AUX_LIBRARIES}
  # ${CAIRO_XCB_LIBRARIES}
  # Unwind dl # for hack_cxa_throw_exception.cpp
  # Pthread
)

#add_subdirectory(tests)
