# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
# @link http://www.vtk.org/Wiki/CMake_Useful_Variables

cmake_minimum_required(VERSION 3.3)

project(FabicCppWanderings
  VERSION 0.0.5
  DESCRIPTION "fabic.net - personal C++ playground project."
  LANGUAGES CXX C
              # ^ for liblmdb.
  )

# set (FabicCppWanderings_VERSION_MAJOR 0)
# set (FabicCppWanderings_VERSION_MINOR 0)
# set (FabicCppWanderings_VERSION_PATCH 5)

# Fancy hello banner.
message(STATUS "~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~")
message(STATUS "~~  Dude, hello! This is CMakeLists.txt of '${CMAKE_CURRENT_SOURCE_DIR}'.")
message(STATUS "~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~")


# Add ./cmake/ and ./cmake/modules/ to CMake module search path.
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  message(STATUS "Dude: Added ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ to CMAKE_MODULE_PATH.")
  if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
    message(STATUS "Dude: Also added ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules to CMAKE_MODULE_PATH.")
    include(dude_please_dump_cmake_variables)
    include(list_all_targets)
  endif()
endif()


###
## Add a new `` MinSizeDeb `` build type configuration.
#
# https://cmake.org/Wiki/CMake_FAQ#How_can_I_specify_my_own_configurations_.28for_generators_that_allow_it.29_.3F
# https://cmake.org/Wiki/CMake_FAQ#How_can_I_extend_the_build_modes_with_a_custom_made_one_.3F
if (true)

  if (NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES  Debug;Release;RelWithDebInfo; CACHE STRING
      "List of CMake build types (for CMAKE_BUILD_TYPE).")
  endif()

  # ~> A) 1st add it to the list of configuration types [optional it appears (?)]
  #       (note that it may not be supported by CMake, seamingly, see FAQ).
  if(CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES MinSizeDeb )
    list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
      "Dude, added MinSizeDeb build configuration, dah!" FORCE)
  endif()

  # ~> B) MinSizeDeb : There it actually is defined.
  set( CMAKE_C_FLAGS_MINSIZEDEB "-Os -g" CACHE STRING
    "Optimize for size, with debug informations (C compiler flags)." FORCE )
  set( CMAKE_CXX_FLAGS_MINSIZEDEB "-Os -g" CACHE STRING
      "Optimize for size, with debug informations (C++ compiler flags)." FORCE )

  set( CMAKE_EXE_LINKER_FLAGS_MINSIZEDEB
      "-Wl,--warn-unresolved-symbols" CACHE STRING
      "Flags used for linking executables (MinSizeDeb)." FORCE )
  set( CMAKE_SHARED_LINKER_FLAGS_MINSIZEDEB
      "-Wl,--warn-unresolved-symbols" CACHE STRING
      "Flags used by the shared libraries linker (MinSizeDeb)." FORCE )
endif()

###
## SET DEFAULT BUILD TYPE ( if not provided by user ).
#
# https://cmake.org/Wiki/CMake_FAQ#How_can_I_change_the_default_build_mode_and_see_it_reflected_in_the_GUI.3F
IF(NOT CMAKE_BUILD_TYPE)
  #SET(CMAKE_BUILD_TYPE MinSizeDeb CACHE STRING
  #SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel MinSizeDeb."
      FORCE)
  message(STATUS "Dude: CMAKE_BUILD_TYPE wasn't specified, setting it to ${CMAKE_BUILD_TYPE}")
  message(STATUS "      ` may be one of: ${CMAKE_CONFIGURATION_TYPES}")
ENDIF(NOT CMAKE_BUILD_TYPE)


###
## CCACHE
# https://crascit.com/2016/04/09/using-ccache-with-cmake/
if (false)
  find_program(CCACHE_PROGRAM ccache)
  if (CCACHE_PROGRAM)
    set_property( GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}" )
    message(STATUS "Dude! Found ccache, so we're setting CMake global property RULE_LAUNCH_COMPILE to '${CCACHE_PROGRAM}'.")
  else()
    message(STATUS "Dude -_- Didn't find `ccache` (which is fine)")
  endif()
else()
  message(STATUS "Dude: ccache compilation cache is _not_ enabled (see this CMakeFiles.txt)")
endif()


## Two custom targets 'debug' and 'release'.
if (true)
  # `cmake --build build --target debug`
  add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Run CMake for a Debug build."
    )
  # `cmake --build build --target release`
  add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Run CMake for a Release build."
    )
  message(STATUS "Dude: Added two convenience targets 'debug' and 'release'.")
  message(STATUS "      ` ex.: $ cmake --build build/ --target release")
endif()

### ~~ local/ FHS-like sub-dir. ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
#
# Global variable DUDE_LOCAL_DIR is set to the abs. path to the `local/` dir.
#
# ./local/{cmake,include,lib}/ : search paths for find_file() and find_library().
if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/local")
  set(DUDE_LOCAL_DIR "${CMAKE_SOURCE_DIR}/local")
  message(STATUS "Dude: Found FHS local/ sub-dir.: ${DUDE_LOCAL_DIR}")

  # Eventual `cmake/` sub-directory.
  list(APPEND CMAKE_MODULE_PATH "${DUDE_LOCAL_DIR}/cmake")
  message( STATUS "Updated CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )

  # include/
  list(APPEND CMAKE_INCLUDE_PATH "${DUDE_LOCAL_DIR}/include")
  message( STATUS "Updated CMAKE_INCLUDE_PATH = ${CMAKE_INCLUDE_PATH}" )
  include_directories( ${DUDE_LOCAL_DIR}/include )

  # lib/
  list(APPEND CMAKE_LIBRARY_PATH "${DUDE_LOCAL_DIR}/lib")
  message( STATUS "Updated CMAKE_LIBRARY_PATH = ${CMAKE_LIBRARY_PATH}" )
  link_directories( "${DUDE_LOCAL_DIR}/lib" )

  # FIXME ? CMake should have inferred this given I didn't define LD_LIBRARY_PATH.
  if (true)
    link_libraries( -Wl,-rpath="${DUDE_LOCAL_DIR}/lib" )
    message(STATUS "Dude: (!) We're setting up linker with `-Wl,-rpath='${DUDE_LOCAL_DIR}/lib'` (!)")
    message(STATUS "Dude:     ` TODO: investigate why you're doing this (!?)")
  endif()

  # FIXME: TEMP (!)
  # FIXME: TEMP (!) CHANGE DEFAULT INSTALL PREFIX.
  # FIXME: TEMP (!)
  if (true)
    if ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" )
      set(CMAKE_INSTALL_PREFIX "${DUDE_LOCAL_DIR}/local")
      message(STATUS "Dude: overriding CMAKE_INSTALL_PREFIX, now set to '${CMAKE_INSTALL_PREFIX}'.")
    endif()
  endif()
endif()


## ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
## ~ ~ ~ ~ LTO ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
## ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

option(DUDE_LETS_TRY_LTO
  "Will pass -flto to Clang (requires among other things: Binutils' ld.gold and LLVM/Clang built with LTO support)."
    OFF)

find_program(LD_GOLD_PROGRAM ld.gold)

if (LD_GOLD_PROGRAM)
  message(STATUS "Dude! Found Binutils' « ld.gold » linker (which supports LTO and the `-plugin ...` argument, btw).")
  # message(STATUS "Setting CMAKE_LINKER = '${LD_GOLD_PROGRAM}'")
  # set( CMAKE_LINKER "${LD_GOLD_PROGRAM}" )
  # ^ DON'T: enabling it for non-LTO compilation/linkage appears to fail with :
  #          “main.cpp.o: unrecognized file format”
endif()

if (DUDE_LETS_TRY_LTO)
  if (LD_GOLD_PROGRAM)
    message(STATUS "Setting CMAKE_LINKER = '${LD_GOLD_PROGRAM}'")
    set( CMAKE_LINKER "${LD_GOLD_PROGRAM}" )

    add_compile_options( -flto )
    link_libraries( -flto )
    message(STATUS "Yeah man! Setting the -flto Clang compiler flag, fingers-crossed o_o")
    message(STATUS "          Setting the -flto linker flag too.")

    # Replace -Os with -O2 from the CXXFLAGS.
    # FIXME: We're not altering the CFLAGS.
    if (true)
      string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
      #message(STATUS "HUH! ${CMAKE_CXX_FLAGS} -- ${uppercase_CMAKE_BUILD_TYPE} --- ${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}}")
      if ( ${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}} MATCHES "-Os" )
        message(STATUS "Beware! Clang won't let us compile with -Os while doing LTO (at least not as of February 2017).")
        string(REPLACE "-Os" "-O2" CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE} "${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}}")
        message(STATUS "Replaced -Os with -O2 from CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE} : ${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}}.")
      endif()
      unset( uppercase_CMAKE_BUILD_TYPE )
    endif()
  else()
    message(WARNING "Argh! dude :-| Couldn't find the `ld.gold` binary (well CMake didn't, I'm cool) ; we're _not_ passing -flto.")
  endif()
endif()

# ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`

message(STATUS "Dude, FYI: CMAKE_BUILD_TYPE     = ${CMAKE_BUILD_TYPE}")
message(STATUS "           CMAKE_MODULE_PATH    = ${CMAKE_MODULE_PATH}")
message(STATUS "           CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "           PROJECT_SOURCE_DIR   = ${PROJECT_SOURCE_DIR}")
message(STATUS "           PROJECT_BINARY_DIR   = ${PROJECT_BINARY_DIR}")

# ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`  ~`



## PKG-CONFIG auto-configuration “CMake bridge” # # # # # # # # # # # #
#
# CMake module: /usr/share/cmake/Modules/FindPkgConfig.cmake
# Cf also: https://wiki.gnome.org/Projects/gtkmm/UsingCMake
find_package(PkgConfig)


##
# This will cause `-I.` and similar/redondant directives like `-I../symfony/..`
# to be emitted (for systematic ability of #include-ing files from the
# “current source file” directory)
#
# EDIT/2016-07-27 : Enabling thing project-wide, having definitions (headers)
#                   separated from the sources provide no added value at the
#                   moment, and gets in my way.
# EDIT/2017-03-12 : Feels not right even though it's common practice – let's try
#                   to _always_ ressort to relative paths _from the src/ sources
#                   root.
#set(CMAKE_INCLUDE_CURRENT_DIR ON)


### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
### #  COMPILER FLAGS, DEFINES, ETC...  # # # # # # # # # # # # # # # #
### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Top-level namespace #define-s
add_definitions()


# Replacing these `set ...` by add_compile_options(...) :
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

# Use Clang's libcxx ?
# add_compile_options( -stdlib=libc++ )
# add_compile_options( -stdlib=libstdc++ )


add_compile_options(
  #-std=c++1z
  # ^ don't do this from this top-level dir.
  #   it gets in the way when compiling C sources, notably.
  #-fPIC
  -Wall
  -pedantic
  # -Weverything
  #   ^ http://clang.llvm.org/docs/UsersManual.html#enabling-all-diagnostics
  )

# /me want.
if (true)
  add_compile_options( -march=amdfam10 )
endif()

add_compile_options(
  -fcolor-diagnostics
  -fdiagnostics-show-category=name
  -fdiagnostics-show-template-tree
  #   ^ http://clang.llvm.org/docs/UsersManual.html#formatting-of-diagnostics
  # -fvisibility=hidden
  #   ^ http://clang.llvm.org/docs/LTOVisibility.html
  #     See Boost.DLL about symbol shadowing problem w/ Linux.
  # -fwhole-program-vtables
  #   ^ http://clang.llvm.org/docs/UsersManual.html#cmdoption-fwhole-program-vtables
  #-Wdocumentation
  #-fparse-all-comments
  #-Wno-documentation-unknown-command
  # ^ http://clang.llvm.org/docs/UsersManual.html#comment-parsing-options
)

# add_compile_options( -flto )
# ^ requires LLVMgold.so, missing on my Sabayon system (Clang 3.8).
#   http://clang.llvm.org/docs/LTOVisibility.html


add_compile_options(
  # -O0 -ggdb
  #      ^ implies -g
  # ^ this is automatically handled by CMake build type thing.
  # So as to get better/meaninful stack traces in error messages :
  -fno-omit-frame-pointer
  # and disable tail call elimination
  #-fno-optimize-sibling-calls
  )


add_compile_options(
  # -fsanitize=address     # http://clang.llvm.org/docs/AddressSanitizer.html
  # ^ -fsanitize=memory
  # ^ -fsanitize=thread
  #   “ It is not possible to combine more than one of the
  #     -fsanitize=address, -fsanitize=thread, and -fsanitize=memory
  #     checkers in the same program. ”
  #     http://clang.llvm.org/docs/UsersManual.html#controlling-code-generation
  # -fsanitize=undefined   # http://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
  # -fsanitize=dataflow    # http://clang.llvm.org/docs/DataFlowSanitizer.html
  # -fsanitize=cfi -flto   # http://clang.llvm.org/docs/ControlFlowIntegrity.html (requries -flto)
  # -fsanitize=safe-stack  # http://clang.llvm.org/docs/SafeStack.html
  )


add_compile_options(
  # -fvisibility-inlines-hidden
  # -Wwrite-strings
  # -Wcast-qual
  -Wmissing-field-initializers
  # -Wcovered-switch-default
  #   ^ we get too many warnings from external headers.
  -Wnon-virtual-dtor
  -Wdelete-non-virtual-dtor
  -Woverloaded-virtual
  # -ffunction-sections
  # -fdata-sections
  # -fno-common
  # -Wno-nested-anon-types
  # -Wl,-allow-shlib-undefined
)


# TODO
# TODO
# TODO
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in")
  message(STATUS "Dude: config.h.in found.")
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
                  ${CMAKE_CURRENT_BINARY_DIR}/config.h    )
endif()


### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Boost C++
#
# https://cmake.org/cmake/help/latest/module/FindBoost.html
# https://svn.boost.org/trac/boost/wiki/CMakeBuildFeatures
if (true)
  # Hard-coded path if needed for `find_package(Boost...)`
  #   - or simply set this as an environment variable,
  #   - or pass `-DBOOSTROOT=...` when running CMake.
  # (works ok with my CMake v3.4.3)
  if (NOT IS_DIRECTORY "$ENV{BOOSTROOT}")
    set(BOOSTROOT ../local/boost-1.61.0-clang)
    message(WARNING "Dude: Boost C++: using hard-coded path for BOOSTROOT = ${BOOSTROOT}.")
  elseif(IS_DIRECTORY $ENV{BOOSTROOT})
    set(BOOSTROOT $ENV{BOOSTROOT})
    message(STATUS "Dude: Boost C++ location said to be at ${BOOSTROOT}")
  else()
    message(WARNING "Dude: Boost C++ location $ENV{BOOSTROOT} ain't no exit buddy.")
  endif()

  # This one (with the '_' blankee) is for JetBrains' CLion bundled CMake (v3.5.2) :
  set(BOOST_ROOT ${BOOSTROOT})

  # These two ain't needed, actually...
  # set(BOOST_INCLUDE_DIRS ${BOOSTROOT}/include)
  # set(BOOST_LIBRARY_DIRS ${BOOSTROOT}/local/boost-1.61.0-clang/lib)

  #set (Boost_DEBUG)
  # ^ this turns out debug output _ONLY FOR_ the CMake module !

  set (Boost_USE_MULTITHREAD ON)  # enable multithreading
  set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking

  #add_definitions(-DBOOST_ALL_DYN_LINK)
  # ^ solves linking problems, notably with Boost.log library.
  #   See http://www.boost.org/doc/libs/1_61_0/libs/log/doc/html/log/rationale/namespace_mangling.html
  #   See also `symfony/include/fabic/logging.hpp`

  #   Note that the following would also solve the
  #     “ unresolved symbol `v2s_mt_posix` ”
  #   problem for the `log` module only.
  add_definitions(-DBOOST_LOG_DYN_LINK)

  # 2018-01-09 : FindBoost has not yet been updated for handling the latest
  #              release; note that this _breaks_ the dependencies resolution
  #              thing (see FindBoost.cmake).
  set(Boost_ADDITIONAL_VERSIONS 106600 )

  find_package(Boost 1.61.0)

  if (Boost_FOUND)
  	include_directories(${Boost_INCLUDE_DIR})
    # find_package (
    #   Boost
    #   COMPONENTS REQUIRED
    #     log
    #     program_options
    #     filesystem
    #     system
    # )
    # ^ now performed where/when/as needed.
  else()
    message(WARNING "Dude: Boost C++: Ain't not found lad :-/")
    message(WARNING "      (it's okay, btw. you have quite an awkward")
    message(WARNING "       love-hate relationship with it, better to")
    message(WARNING "       take a step back away from it from time to time).")
  endif()
endif()
# ^ Boost C++
###


###
# cpp-netlib
#
# For the HTTP module. Defined here instead of within `http/CMakeLists.txt`
# so that we may eventually have other modules based on cpp-netlib.
#
# Defined here :
#   - ${LibCppNetlibClientConnections}
#   - ${LibCppNetlibServerParsers}
#
# FIXME: hard-coded paths, can't get my way 'round those CMake stuff...
if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib/boost)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib)

  find_file(
    LibCppNetlibClientConnections
    libcppnetlib-client-connections.so
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib/build/libs/network/src
    )

  find_file(
    LibCppNetlibServerParsers
    libcppnetlib-server-parsers.so
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-netlib/build/libs/network/src
    )
endif()
# ^ cpp-netlib
##

###
## LLVM/Clang
#
#find_package(LLVM REQUIRED CONFIG)
if (true)
  find_package(LLVM CONFIG)
  # find_package(Clang REQUIRED)

  # This would typically append for ex. '/usr/lib/cmake/llvm/' to CMake search path.
  list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

  # For `add_llvm_loadable_module()` notably.
  include(AddLLVM)

  message(STATUS "Dude: Found LLVM version ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Dude: Using LLVMConfig.cmake found at: ${LLVM_DIR}")

  # Don't (!): do this as needed.
  #add_definitions( ${LLVM_DEFINITIONS} )
  #include_directories( ${LLVM_INCLUDE_DIRS} )
endif()


# TODO: Set this up!? see experiments/010/CMakeLists.txt
# LMDB : FIXME: don't add_subdirectory(): it inherits stuff that we do here, like compiler flags etc..
if (false)
  find_library(StaticLibLMDB lmdb)
  if ( StaticLibLMDB MATCHES "-NOTFOUND$" )
    message(STATUS "Dude: Couldn't find LMDB's static liblmdb.a")
    add_subdirectory( vendor/lmdb )
  else()
    message(STATUS "Dude: Found LMDB static lib.: ${StaticLibLMDB}")
  endif()
endif()

## Third-party libraries under lib/ or vendor/
if (true)
  # lib/
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/CMakeLists.txt")
    message(STATUS "Dude: Adding subdirectory lib/ (third-party stuff).")
    add_subdirectory( lib )
  endif()
  # vendor/
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/CMakeLists.txt")
    message(STATUS "Dude: Adding subdirectory vendor/ (third-party stuff).")
    add_subdirectory( vendor )
  endif()
endif()

###  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #
## Source code layout :
#  - Either we have an INCLUDE/ dir. with headers separated from the
#    implementations in some SRC/ dir.
#  - Or we just have a SRC/ dir. => in which case we add it as an include (-I)
#    directory.
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
  include_directories( include )
  # include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
  message(STATUS "Dude: Found your include/ dir.")
elseif (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
  include_directories( src )
  message(STATUS "FYI: Adding src/ as compiler include (-I) path; since there's no include/ subdir. here")
endif()


# Add the src/ subdirectory only if it has a CMakeLists.txt file.
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt")
    add_subdirectory( src )
    message(STATUS "FYI: Added subdirectory src/")
  else()
    message(WARNING "Dude! Found that src/ subdir. of yours, _BUT_ got no src/CMakeLists.txt (!) Skipping...")
  endif()
endif()

add_subdirectory(experiments)
# add_subdirectory(lib)

# `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~
# `~. BOTTOM OF THAT CMAKE CONFIGURATION FILE `~. `~. `~. `~. `~. `~. `~. `~. `~
# `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~. `~

# Custom target that list executable files.
if (false)
  add_custom_target( list-binaries
    #ALL
    DEPENDS clong-05 # FIXME: last target !
      COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -type d -name CMakeFiles -prune -o -type f -perm -a+x -ls
      COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -type d -name CMakeFiles -prune -o -type f -name '*.a' -ls
      COMMAND echo -e "\\e[32;1;7m  HEY!  \\e[0m"
      COMMAND echo -e "\\e[2m  Dude, have a break away from computer at times!  \\e[0m"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "-- List os executable files (+x) that were built : --"
    VERBATIM USES_TERMINAL
    )
endif()

# TODO: have a custom target that runs built program under LLDB.
#       $ lldb -o 'run' -- ls -lad

if (false)
  dude_list_all_targets(${CMAKE_CURRENT_SOURCE_DIR} _all_library_targets _all_executable_targets _all_other_targets)
  message(STATUS "All library targets: ${_all_library_targets}")
  message(STATUS "All executable targets: ${_all_executable_targets}")
  message(STATUS "All other targets: ${_all_other_targets}")
endif()

message(STATUS "~~")
message(STATUS "~~ Dude: We're done with ${CMAKE_CURRENT_LIST_FILE}.")
message(STATUS "~~")
message(STATUS "~~       Remember to RELAX and BREATHE, this is just code,")
message(STATUS "~~       don't take these matters too seriously (!)")
message(STATUS "~~       and have fun !  ^_^")
message(STATUS "~~")
message(STATUS "~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~")
