
cmake_minimum_required(VERSION 3.3)

add_executable(main1    main1.cpp)
add_executable(mixed    mixed.cpp)
add_executable(syscall1 syscall1.cpp)

# now fails to link (2016-07-11)
if (false)
  add_executable(debug_boost_options_encoding
    boost_options_debug_encoding.cpp
    ../../src/hacks/hack_cxa_throw_exception.cpp
  )
endif (false)
