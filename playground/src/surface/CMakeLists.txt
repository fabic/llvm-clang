
cmake_minimum_required(VERSION 3.3)

project(Surface)


find_package (
  Boost
  COMPONENTS REQUIRED
    log
    # program_options
    # filesystem
    # system
  )


pkg_check_modules(XCB xcb)
# FYI: ^ redundant,
#        implied by xcb-aux it appears :
pkg_check_modules(XCB_AUX xcb-aux)

pkg_check_modules(CAIRO_XCB cairo-xcb)

pkg_check_modules(FT2 freetype2)


add_executable( sf
    main.cpp
    xcb/xcb.cpp
    xcb/events.cpp
    xcb/window.cpp
    cairo/surface.cpp
    ui/elements.cpp
    ui/surface.cpp
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
