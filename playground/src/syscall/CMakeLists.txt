
cmake_minimum_required(VERSION 3.3)

project(FabicSyscalls)

set (FabicSyscalls_VERSION_MAJOR 0)
set (FabicSyscalls_VERSION_MINOR 1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions()

add_compile_options(
  -std=c++1z
  #-stdlib=libc++
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

add_compile_options(
  # -O0 -ggdb
  # ^ this is automatically handled by CMake build type thing.

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
  -fcolor-diagnostics
  # -ffunction-sections
  # -fdata-sections
  # -fno-common
  # -Wno-nested-anon-types
  # -Wl,-allow-shlib-undefined
)

# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ TARGETS ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

add_executable( test_aab tests/test_aab.cpp )
target_compile_options( test_aab PUBLIC -nostdinc -nostdinc++ )
# NOTE: Clang also has -nobuiltininc but complains that is wasn't used during compilation.

add_executable( test_aac tests/test_aac.cpp assert.cpp )
target_compile_options( test_aac PUBLIC -nostdinc -nostdinc++ )
set_target_properties( test_aac PROPERTIES LINK_FLAGS -nostdlib)



add_executable( aaa
  tests/test001.cpp
  #../hacks/hack_cxa_throw_exception.cpp
  )


target_include_directories( aaa
  PUBLIC
  # ...
)

target_link_libraries( aaa
  # ${Boost_LIBRARIES}
  # Unwind dl # for hack_cxa_throw_exception.cpp
)

#add_subdirectory(tests)
