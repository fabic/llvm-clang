
add_executable( sf-xcb-window-test
    xcb-window.cpp
    ../xcb/events.cpp
    ../xcb/xcb.cpp
    ../xcb/window.cpp
    #../cairo/cairo.cpp
    #../hacks/hack_cxa_throw_exception.cpp
)

target_compile_options( sf-xcb-window-test
  PUBLIC
  # etc...
    )

target_include_directories( sf-xcb-window-test
  PUBLIC
    ${XCB_INCLUDE_DIRS}
    ${XCB_AUX_INCLUDE_DIRS}
    ${CAIRO_XCB_INCLUDE_DIRS}
)

target_link_libraries( sf-xcb-window-test
  ${Boost_LIBRARIES}
  ${XCB_LIBRARIES}
  ${XCB_AUX_LIBRARIES}
  ${CAIRO_XCB_LIBRARIES}
  unwind dl # for hack_cxa_throw_exception.cpp
  pthread
)

