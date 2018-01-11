cmake_minimum_required(VERSION 3.3)

# Entails -I. & -I../experiments etc... for current dir. searched first.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# BINARY: build/experiment/gtk
  pkg_check_modules(GTKMM gtkmm-3.0)

  if (GTKMM_FOUND AND FALSE)
    # FIXME: it appears CMake doesn't have a target_link_directories().
    link_directories(
      ${GTKMM_LIBRARY_DIRS}
      )

    add_executable(
      gtk
      gtkmm.cpp
      ../../src/dude/hacks/hack_cxa_throw_exception.cpp
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
  endif()
# ^ GTK
###


## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# BINARY: build/experiments/xlib

  # /usr/share/apps/cmake/modules/FindX11.cmake
  find_package(X11)

  if (X11_FOUND)
    add_executable(
      xlib
      xlib1.cpp
      ../../src/dude/hacks/hack_cxa_throw_exception.cpp
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
  endif()

# ^ Xlib
###


## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# BINARY: build/experiments/xcb1
#
# https://www.freetype.org/freetype2/docs/documentation.html
# http://linuxfromscratch.org/blfs/view/svn/general/freetype2.html

  pkg_check_modules(XCB xcb)
  pkg_check_modules(XCB_AUX xcb-aux)
  pkg_check_modules(FT2 freetype2)
  pkg_check_modules(CAIROMM cairomm-1.0)

  if (XCB_FOUND)
    add_executable(
      xcb1
      xcb1.cpp
      ../../src/dude/hacks/hack_cxa_throw_exception.cpp
    )

    target_compile_options(
      xcb1
      PUBLIC
      # ...
      )

    target_include_directories(
      xcb1
      PUBLIC
      ${XCB_INCLUDE_DIRS}
      )

    target_link_libraries(
      xcb1
      ${Boost_LIBRARIES}
      ${XCB_LIBRARIES}
      unwind dl # for hack_cxa_throw_exception.cpp
      pthread
      )
  endif()
# ^ Xcb1
###
