
cmake_minimum_required(VERSION 3.3)

project(Syscalls)src/syscall/test_aab

add_executable( test_aab tests/test_aab.cpp )

add_executable( aaa
    tests/test001.cpp
    #../hacks/hack_cxa_throw_exception.cpp
    )

target_compile_options( aaa
  PUBLIC
  # ...
)

target_include_directories( aaa
  PUBLIC
  # ...
)

target_link_libraries( sf
  # ${Boost_LIBRARIES}
  # Unwind dl # for hack_cxa_throw_exception.cpp
)

#add_subdirectory(tests)
