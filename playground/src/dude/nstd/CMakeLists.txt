
cmake_minimum_required(VERSION 3.3)

# MinSizeDeb build type
set( CMAKE_C_FLAGS_MINSIZEDEB "-Oz -g" CACHE STRING
  "Optimize for size, with debug informations (C compiler flags)." FORCE )
set( CMAKE_CXX_FLAGS_MINSIZEDEB "-Oz -g" CACHE STRING
    "Optimize for size, with debug informations (C++ compiler flags)." FORCE )
set( CMAKE_EXE_LINKER_FLAGS_MINSIZEDEB
    "-Wl,--warn-unresolved-symbols" CACHE STRING
    "Flags used for linking executables (MinSizeDev)." FORCE )
set( CMAKE_SHARED_LINKER_FLAGS_MINSIZEDEB
    "-Wl,--warn-unresolved-symbols" CACHE STRING
    "Flags used by the shared libraries linker (MinSizeDeb)." FORCE )

## Set default build type.
# https://cmake.org/Wiki/CMake_FAQ#How_can_I_change_the_default_build_mode_and_see_it_reflected_in_the_GUI.3F
if(NOT CMAKE_BUILD_TYPE)
  #SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
  set(CMAKE_BUILD_TYPE MinSizeDeb CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel MinSizeDeb."
    FORCE)
endif(NOT CMAKE_BUILD_TYPE)

##
##
##

project(NStd)

set (NStd_VERSION_MAJOR 0)
set (NStd_VERSION_MINOR 1)

if(NOT "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86")
  message(WARNING "Dude! Ain't no x86 system here ?!")
endif()

if(false)
  # https://cmake.org/Wiki/CMake/Assembler
  enable_language(ASM-ATT)

  if (NOT CMAKE_ASM-ATT_COMPILER_WORKS)
    message(WARNING "Dude! CMake says the assembler does not work -_-")
  endif()
endif()

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# ^ DON'T ! (it sucks)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions()

add_compile_options(
  -std=c++1z
  #-stdlib=libc++
  -fPIC
  -Wall
  -pedantic
  #-Weverything
  #   ^ http://clang.llvm.org/docs/UsersManual.html#enabling-all-diagnostics
  )

add_compile_options(
  -fcolor-diagnostics
  -fdiagnostics-show-category=name
  -fdiagnostics-show-template-tree
  # ^ http://clang.llvm.org/docs/UsersManual.html#formatting-of-diagnostics

  # -fvisibility=hidden
  #   ^ http://clang.llvm.org/docs/LTOVisibility.html
  #     See Boost.DLL about symbol shadowing problem w/ Linux.

  # -fwhole-program-vtables
  #   ^ http://clang.llvm.org/docs/UsersManual.html#cmdoption-fwhole-program-vtables
)

add_compile_options(
  # So as to get better/meaninful stack traces in error messages :
  -fno-omit-frame-pointer
  # and disable tail call elimination
  -fno-optimize-sibling-calls
  )

add_compile_options(
  # -fvisibility-inlines-hidden
  # -Wwrite-strings
  # -Wcast-qual
  -Wmissing-field-initializers
  -Wcovered-switch-default
  -Wnon-virtual-dtor
  -Wdelete-non-virtual-dtor
  -Woverloaded-virtual
  # -ffunction-sections
  # -fdata-sections
  # -fno-common
  # -Wno-nested-anon-types
  # -Wl,-allow-shlib-undefined
)

# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ TARGETS ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

#add_compile_options( -nostdinc -nostdinc++ )
# NOTE: Clang also has `-nobuiltininc` but complains
#       that is wasn't used during compilation.

# Prevents Clang from issuing a warning :
#   “ISO C++ does not allow 'main' to be used by a program [-Wmain,Semantic Issue]”
set_source_files_properties( c/_start.cpp PROPERTIES COMPILE_FLAGS -Wno-main )

# We need an object file `nstd_start.o` that has to be linked along programs ;
# (basically this is our custom `crt1.o` program low-level bootstrapping "pipework".
add_library( nstd_start OBJECT c/_start.cpp )
add_library( nstd_crti  OBJECT c/_init_n_fini_prolog.cpp )
add_library( nstd_crtn  OBJECT c/_init_n_fini_epilog.cpp )

# ~ ~ ~ Das `nstd` lib. ~ ~ ~
add_library( nstd SHARED
    #c/_start.cpp
    #FIXME: $<TARGET_OBJECTS:nstd_crti>
    $<TARGET_OBJECTS:nstd_start>
    c/memset.cpp
    c/assert.cpp
    c/atexit.cpp
    unwind/unwind.cpp
    eh/cxa.cpp
    malloc/malloc.cpp
    String/length.cpp
    Process.cpp
    File.cpp
    kernel/linux/errno.cpp
    #FIXME: $<TARGET_OBJECTS:nstd_crtn>
  )
target_compile_options( nstd PUBLIC -nostdinc -nostdinc++ )
set_target_properties( nstd PROPERTIES LINK_FLAGS -nostdlib)

#link_libraries( -nostdlib nstd )
#               ^ manual says it's ok to pass linker flags in this way. )
#                 Alternate solution was :
#set_target_properties( test_aac PROPERTIES LINK_FLAGS -nostdlib)



# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ TESTS & DEV. PROTOTYPES ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

# add_subdirectory(tests)

# add_executable( test_aab tests/test_aab.)pp )
# add_executable( test_aac tests/test_aac.cpp )
# ...

if (true)
  set(test_sources_list
      tests/test_aab.cpp
      tests/test_aac.cpp
      tests/test_aad.cpp
      tests/test_aae.cpp
      tests/test_aaf.cpp
      tests/test_sys_open.cpp
      String/test_length.cpp
    )

  foreach(file ${test_sources_list})
    message(STATUS "Huh! ${file}")
    get_filename_component(file_we ${file} NAME_WE)
    # Link against our custom crt1.o thing :
    add_executable( ${file_we}
        #FIXME: $<TARGET_OBJECTS:nstd_crti>
        $<TARGET_OBJECTS:nstd_start>
        ${file}
        #FIXME: $<TARGET_OBJECTS:nstd_crtn>
      )
    target_compile_options( ${file_we} PUBLIC -nostdinc -nostdinc++ )
    target_link_libraries( ${file_we} -nostdlib nstd)
  endforeach()

  # Initial dev/test impl. of _start & _start_c
  set_source_files_properties( tests/test_aac.cpp PROPERTIES COMPILE_FLAGS -Wno-main )

  add_executable(test001 tests/test001.cpp)
  target_link_libraries( ${file_we} nstd)
endif()

# ~ ~ ~ * ~ ~ ~

# Old thing that is.
if (false)
  add_executable( aaa tests/test001.cpp
      #../hacks/hack_cxa_throw_exception.cpp
    )
  target_include_directories( aaa PUBLIC )
  target_link_libraries( aaa
      # ${Boost_LIBRARIES}
      # Unwind dl # for hack_cxa_throw_exception.cpp
    )
endif()
