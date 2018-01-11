
cmake_minimum_required(VERSION 3.3)

project(Dumbster)

set (Dumbster_VERSION_MAJOR 0)
set (Dumbster_VERSION_MINOR 1)


# ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
# DEPENDENCIES
# ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
# find_package( LLVM  REQUIRED CONFIG )
# find_package( Clang REQUIRED )
find_package( Boost COMPONENTS REQUIRED
    log
    # program_options
    filesystem
    system
  )

# message(STATUS "Clong: Found LLVM ${LLVM_PACKAGE_VERSION}")
# message(STATUS "Clong: Using LLVMConfig.cmake in: ${LLVM_DIR}")

# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ TARGETS ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

add_executable( dumbster
    main.cpp
    parser.cpp
    fragments.cpp
    ../lexer/lexer.cpp
    ../lexer/token.cpp
    ../filesystem/file.cpp
    ../hacks/hack_cxa_throw_exception.cpp
  )

# llvm_map_components_to_libnames(
#     LLVM_CLANG_LIBS
#     Support Option
# )

target_link_libraries( dumbster
    # ${LLVM_CLANG_LIBS}
    ${Boost_LIBRARIES}
    unwind dl # for hack_cxa_throw_exception.cpp
    pthread
  )

# dude_please_dump_cmake_variables()
