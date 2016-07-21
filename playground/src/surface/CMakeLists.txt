
cmake_minimum_required(VERSION 3.3)

project(Surface)

# FIXME
#include_directories(../symfony/include)

# pkg_check_modules(GTKMM gtkmm-3.0)
# if (GTKMM_FOUND)
# endif (GTKMM_FOUND)

# find_package(X11)
# if (X11_FOUND)
# endif (X11_FOUND)

# pkg_check_modules(CAIROMM cairomm-1.0)


## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# BINARY: build/experiments/xcb
#
# https://www.freetype.org/freetype2/docs/documentation.html
# http://linuxfromscratch.org/blfs/view/svn/general/freetype2.html

pkg_check_modules(XCB xcb)
# FYI: ^ redundant, implied by xcb-aux it appears :
pkg_check_modules(XCB_AUX xcb-aux)

pkg_check_modules(CAIRO_XCB cairo-xcb)

pkg_check_modules(FT2 freetype2)


add_executable( sf
    main.cpp
    surface.cpp
    xcb/xcb.cpp
    xcb/events.cpp
    xcb/window.cpp
    cairo/cairo.cpp
    ui/elements.cpp
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
