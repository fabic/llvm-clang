# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
# @link http://www.vtk.org/Wiki/CMake_Useful_Variables
#
cmake_minimum_required(VERSION 3.3)

project(FabicCppWanderings)

set (FabicCppWanderings_VERSION_MAJOR 0)
set (FabicCppWanderings_VERSION_MINOR 1)

# Top-level namespace #define-s
add_definitions(
  -DTL_NS_BEGIN=
  -DTL_NS_END=
  -DTLNS=
  )

# This will cause `-I.` and similar/redondant directives like `-I../symfony/..`
# to be emitted (for systematic ability of #include-ing files from the
# “current source file” directory)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Replacing these `set ...` by add_compile_options(...) :
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++ -Wall -pedantic")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")


add_compile_options(
  -std=c++1z
  -stdlib=libc++
  -fPIC
  -Wall
  -pedantic
  # -Weverything
  #   ^ http://clang.llvm.org/docs/UsersManual.html#enabling-all-diagnostics
  )

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
)

# add_compile_options( -flto )
# ^ requires LLVMgold.so, missing on my Sabayon system (Clang 3.8).
#   http://clang.llvm.org/docs/LTOVisibility.html

add_compile_options(
  -O0
  -ggdb
    # So as to get better/meaninful stack traces in error messages :
    -fno-omit-frame-pointer
    # and disable tail call elimination
    -fno-optimize-sibling-calls
  )
# ^ implies -g

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
  #-fsanitize=cfi -flto  # http://clang.llvm.org/docs/ControlFlowIntegrity.html (requries -flto)
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
  -fcolor-diagnostics
  # -ffunction-sections
  # -fdata-sections
  # -fno-common
  # -Wno-nested-anon-types
  # -Wl,-allow-shlib-undefined
)


### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Boost C++
#
# https://cmake.org/cmake/help/latest/module/FindBoost.html
# https://svn.boost.org/trac/boost/wiki/CMakeBuildFeatures

  # Hard-coded path if needed for `find_package(Boost...)`
  #   - or simply set this as an environment variable,
  #   - or pass `-DBOOSTROOT=...` when running CMake.
  # (works ok with my CMake v3.4.3)
  set(BOOSTROOT ../local/boost-1.61.0-clang)

  # This one is for JetBrains' CLion bundled CMake (v3.5.2) :
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

  find_package(Boost 1.61.0 REQUIRED)

  if (Boost_FOUND)
  	include_directories(${Boost_INCLUDE_DIR})
  	find_package (
      Boost
      COMPONENTS REQUIRED
        log
        #log_setup
        # ^ ain't needed it appears.
        program_options
        filesystem
        system
    )
  #else
  #  message("Ain't find no Boost C++ lad :-/")
  endif (Boost_FOUND)
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

  include_directories(${CMAKE_SOURCE_DIR}/../misc/cpp-netlib)
  include_directories(${CMAKE_SOURCE_DIR}/../misc/cpp-netlib/deps/uri/include)

  find_file(
    LibCppNetlibClientConnections
    libcppnetlib-client-connections.so
    PATHS ${CMAKE_SOURCE_DIR}/../misc/cpp-netlib/build/libs/network/src
    )

  find_file(
    LibCppNetlibServerParsers
    libcppnetlib-server-parsers.so
    PATHS ${CMAKE_SOURCE_DIR}/../misc/cpp-netlib/build/libs/network/src
    )
# ^ cpp-netlib
##


## pkg-config auto-configuration “CMake bridge” # # # # # # # # # # # # # # #
#
# CMake module: /usr/share/cmake/Modules/FindPkgConfig.cmake
# Defines: GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES.
# Cf also: https://wiki.gnome.org/Projects/gtkmm/UsingCMake
find_package(PkgConfig)

include_directories(include)
add_subdirectory(src)
add_subdirectory(experiments)
#add_subdirectory(clang-check)


### DUMP ALL VARIABLES (DEBUG) ##############################################
##
# http://stackoverflow.com/questions/9298278/cmake-print-out-all-accessible-variables-in-a-script

  if (false)
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
     message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
  endif (false)
##
### ^ CMake defined variables debug dump. ###################################
