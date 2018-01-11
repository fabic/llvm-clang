
cmake_minimum_required(VERSION 3.3)

project(Frog)

set (Frog_VERSION_MAJOR 0)
set (Frog_VERSION_MINOR 1)

# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
# DEPENDENCIES
# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
find_package (
    Boost
    COMPONENTS REQUIRED
      log
      program_options
      filesystem
      system
  )


# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
# TARGETS
# ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

add_executable( frog
    main.cpp
    frog.cpp
    ../hacks/hack_cxa_throw_exception.cpp
  )

target_link_libraries( frog
    ${Boost_LIBRARIES}
    unwind dl # for hack_cxa_throw_exception.cpp
    pthread
  )

