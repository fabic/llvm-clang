# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
# @link http://www.vtk.org/Wiki/CMake_Useful_Variables

cmake_minimum_required(VERSION 3.3)

project(FabicCppWanderings)

set (FabicCppWanderings_VERSION_MAJOR 0)
set (FabicCppWanderings_VERSION_MINOR 1)


## FHS-style local/ directory.
#
  if (NOT DEFINED FABIC_LOCAL_DIR)
    set(FABIC_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../local)
    message(STATUS "Trying FHS-style local/ directory (guessed) : '${FABIC_LOCAL_DIR}'.")
  else()
    message(STATUS "FHS-style local/ directory set to ${FABIC_LOCAL_DIR}")
  endif()

  if (NOT IS_DIRECTORY ${FABIC_LOCAL_DIR})
    message(WARNING "FHS-like “local/” directory doesn't exist.")
  else()
    message(STATUS "FHS-style local/ directory exists, ok.")
  endif()
# ^ local/ FHS-style setup.
##


### LLVM/Clang CMake modules # # # # # # # # # # # # # # # # # # # # # # # # #
##
# TODO: auto-setup CMAKE_MODULE_PATH w/ `for ${FABIC_LOCAL_DIR}/share/*/cmake/`

  if (IS_DIRECTORY ${FABIC_LOCAL_DIR}/share/llvm)
    list(APPEND CMAKE_MODULE_PATH "${FABIC_LOCAL_DIR}/share/llvm/cmake")
    message(STATUS "LLVM: Prepended CMAKE_MODULE_PATH with '${FABIC_LOCAL_DIR}/share/llvm/cmake', ok.")

    # LLVM found CMake modules found, ensure we do have those for Clang too.
    if (IS_DIRECTORY ${FABIC_LOCAL_DIR}/share/clang)
      list(APPEND CMAKE_MODULE_PATH "${FABIC_LOCAL_DIR}/share/clang/cmake")
      message(STATUS "LLVM/Clang: Prepended CMAKE_MODULE_PATH with '${FABIC_LOCAL_DIR}/share/clang/cmake', ok.")
    else()
      message(WARNING "LLVM/Clang: couldn't find '${FABIC_LOCAL_DIR}/share/clang/cmake' (CMake modules for Clang).")
    endif()
  else()
    message(WARNING "LLVM: couldn't find '${FABIC_LOCAL_DIR}/share/llvm/cmake' (CMake modules for LLVM).")
    message(WARNING "LLVM/Clang: not setting up CMAKE_MODULE_PATH.")
  endif()

  message(STATUS "FYI: CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
# ^ LLVM/Clang CMake modules search-path setup.
##


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
#                   moment.
set(CMAKE_INCLUDE_CURRENT_DIR ON)


### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
### #  COMPILER FLAGS, DEFINES, ETC...  # # # # # # # # # # # # # # # #
### # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Top-level namespace #define-s
# FIXME: drop this.
add_definitions(
  -DTL_NS_BEGIN=
  -DTL_NS_END=
  -DTLNS=
  )


# Replacing these `set ...` by add_compile_options(...) :
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")


add_compile_options(
  # -std=c++1z
  # -stdlib=libc++
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
  # -O0 -ggdb
  # ^ this is automatically handled by CMake build type thing.
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
  if (NOT IS_DIRECTORY "$ENV{BOOSTROOT}")
    set(BOOSTROOT ../local/boost-1.61.0-clang)
    message(WARNING "Boost C++: using hard-coded path for BOOSTROOT = ${BOOSTROOT}.")
  elseif(IS_DIRECTORY $ENV{BOOSTROOT})
    set(BOOSTROOT $ENV{BOOSTROOT})
    message(STATUS "Boost C++ location said to be at ${BOOSTROOT}")
  else()
    message(WARNING "Boost C++ location $ENV{BOOSTROOT} ain't no exit buddy.")
  endif()

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
    # TODO: do this where it is actually needed.
  	find_package (
      Boost
      COMPONENTS REQUIRED
        log
        program_options
        filesystem
        system
    )
  else()
    message(WARNING "Boost C++: Ain't not found lad :-/")
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
if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../misc/cpp-netlib/boost)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../misc/cpp-netlib)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../misc/cpp-netlib/deps/uri/include)

  find_file(
    LibCppNetlibClientConnections
    libcppnetlib-client-connections.so
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../misc/cpp-netlib/build/libs/network/src
    )

  find_file(
    LibCppNetlibServerParsers
    libcppnetlib-server-parsers.so
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../misc/cpp-netlib/build/libs/network/src
    )
endif()
# ^ cpp-netlib
##


include_directories(include)
# ^ NOTE/2016-07-27: emptied the include/ dir. in favor of having headers reside
#                    along implementations.

add_subdirectory(experiments)

add_subdirectory(lib)

add_subdirectory(src)


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
