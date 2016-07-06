# cpp_playground/CMakeLists.txt
#
# FabiC's C++ playground
#
# @link https://cmake.org/cmake-tutorial/
#
cmake_minimum_required(VERSION 3.3)

project(FabicCppPlayground)

set (FabicCppPlayground_VERSION_MAJOR 0)
set (FabicCppPlayground_VERSION_MINOR 1)


# This will cause -I. and similar/redondant directiveslike -I../symfony/..
# to be emitted.
# set(CMAKE_INCLUDE_CURRENT_DIR ON)


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

###
# Boost C++
#
# https://cmake.org/cmake/help/latest/module/FindBoost.html
# https://svn.boost.org/trac/boost/wiki/CMakeBuildFeatures

  # Hard-coded path if needed for `find_package(Boost...)`
  #   - or simply set this as an environment variable,
  #   - or pass `-DBOOSTROOT=...` when running CMake.
  # (works ok with my CMake v3.4.3)
  #set(BOOSTROOT ../local/boost-1.61.0-clang)

  # This one is for JetBrains' CLion bundled CMake (v3.5.2) :
  set(BOOST_ROOT ${BOOSTROOT})

  # These two ain't needed, actually...
  # set(BOOST_INCLUDE_DIRS ${BOOSTROOT}/include)
  # set(BOOST_LIBRARY_DIRS ${BOOSTROOT}/local/boost-1.61.0-clang/lib)

  #set (Boost_DEBUG)
  # ^ this turns out debug output _only for the CMake module!_.

  set (Boost_USE_MULTITHREAD ON)  # enable multithreading
  set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking

  #add_definitions(-DBOOST_ALL_DYN_LINK)
  # ^ solves linking problems, notably with Boost.log library.
  #   See http://www.boost.org/doc/libs/1_61_0/libs/log/doc/html/log/rationale/namespace_mangling.html
  #   See also `symfony/include/fabic/logging.hpp`
  # ^ Note that the following would also solve the "unresolved symbol `v2s_mt_posix`"
  #   problem _for the `log` module only.
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
  endif()

# ^ Boost C++
###


add_subdirectory(symfony)
add_subdirectory(surface)
# add_subdirectory(clang-check)
