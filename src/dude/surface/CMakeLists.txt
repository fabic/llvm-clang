
cmake_minimum_required(VERSION 3.3)

project(Surface)

set (Surface_VERSION_MAJOR 0)
set (Surface_VERSION_MINOR 1)

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
    #-fno-omit-frame-pointer
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


# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ DEPENDENCIES ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

find_package (
    Boost
    COMPONENTS REQUIRED
      log
      # program_options
      # filesystem
      # system
  )

## PKG-CONFIG auto-configuration “CMake bridge” # # # # # # # # # # # #
#
# CMake module: /usr/share/cmake/Modules/FindPkgConfig.cmake
# Cf also: https://wiki.gnome.org/Projects/gtkmm/UsingCMake
find_package(PkgConfig)

pkg_check_modules(XCB xcb)
# FYI: ^ redundant,
#        implied by xcb-aux it appears :
pkg_check_modules(XCB_AUX xcb-aux)

pkg_check_modules(CAIRO_XCB cairo-xcb)

pkg_check_modules(FT2 freetype2)


# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
# ~ ~ ~ TARGETS ~ ~ ~
# ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

add_executable( sf
    main.cpp
    xcb/xcb.cpp
    xcb/events.cpp
    xcb/window.cpp
    cairo/surface.cpp
    ui/elements.cpp
    ui/surface.cpp
    text/TextBuffer.cpp
    text/TextBuffers.cpp
    text/Line.cpp ui/TextView.cpp
    ../hacks/hack_cxa_throw_exception.cpp
  )

target_compile_options( sf
    PUBLIC
    # ...
  )

target_include_directories( sf
  PUBLIC
    ${XCB_INCLUDE_DIRS}
    ${XCB_AUX_INCLUDE_DIRS}
    ${CAIRO_XCB_INCLUDE_DIRS}
  )

target_link_libraries( sf
    ${Boost_LIBRARIES}
    ${XCB_LIBRARIES}
    ${XCB_AUX_LIBRARIES}
    ${CAIRO_XCB_LIBRARIES}
    unwind dl # for hack_cxa_throw_exception.cpp
    pthread
  )

add_subdirectory(tests)
