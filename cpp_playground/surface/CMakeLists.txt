# cpp_playground/surface/CMakeLists.txt
#
cmake_minimum_required(VERSION 3.3)

project(Surface)

include_directories(include)

## GTKmm & al. CMake setup howto.
#   https://wiki.gnome.org/Projects/gtkmm/UsingCMake
#
# CMake module: /usr/share/cmake/Modules/FindPkgConfig.cmake
# Defines: GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES.
  find_package(PkgConfig)


  # include_directories(
  #   ${GTKMM_INCLUDE_DIRS}
  #   )
# ^ GTKmm.
##


## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# BINARY: build/surface/gtk
  pkg_check_modules(GTKMM gtkmm-3.0)

  if (GTKMM_FOUND)
    # FIXME: it appears CMake doesn't have a target_link_directories().
    link_directories(
      ${GTKMM_LIBRARY_DIRS}
      )

    add_executable(
      gtk
      src/gtkmm.cpp
      ../symfony/src/hacks/hack_cxa_throw_exception.cpp
    )

    target_compile_options(
      gtk
      PUBLIC
      # ...
      )

    target_include_directories(
      gtk
      PUBLIC
      ${GTKMM_INCLUDE_DIRS}
      )

    target_link_libraries(
      gtk
      ${Boost_LIBRARIES}
      ${GTKMM_LIBRARIES}
      unwind dl # for hack_cxa_throw_exception.cpp
      pthread
      )
  endif (GTKMM_FOUND)

# ^ Surface / GTKmm
###


## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# BINARY: build/surface/xlib

  # /usr/share/apps/cmake/modules/FindX11.cmake
  find_package(X11)

  if (X11_FOUND)
    add_executable(
      xlib
      src/xlib.cpp
      ../symfony/src/hacks/hack_cxa_throw_exception.cpp
    )

    target_compile_options(
      xlib
      PUBLIC
      # ...
      )

    target_include_directories(
      xlib
      PUBLIC
      ${X11_INCLUDE_DIR}
      )

    target_link_libraries(
      xlib
      ${Boost_LIBRARIES}
      ${X11_LIBRARIES}
      unwind dl # for hack_cxa_throw_exception.cpp
      pthread
      )
  endif (X11_FOUND)

# ^ Surface / Xlib
###



## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# BINARY: build/surface/xcb

  # /usr/share/apps/cmake/modules/FindX11.cmake
  pkg_check_modules(XCB xcb)

  if (XCB_FOUND)
    add_executable(
      x11-xcb
      src/xcb.cpp
      ../symfony/src/hacks/hack_cxa_throw_exception.cpp
    )

    target_compile_options(
      x11-xcb
      PUBLIC
      # ...
      )

    target_include_directories(
      x11-xcb
      PUBLIC
      ${XCB_INCLUDE_DIRS}
      )

    target_link_libraries(
      x11-xcb
      ${Boost_LIBRARIES}
      ${XCB_LIBRARIES}
      unwind dl # for hack_cxa_throw_exception.cpp
      pthread
      )
  endif (XCB_FOUND)

# ^ Surface / Xlib
###
