
add_executable( sf-xcb-window-test
    xcb-window.cpp
    ../xcb/events.cpp
    ../xcb/xcb.cpp
    ../xcb/window.cpp
    ../../hacks/hack_cxa_throw_exception.cpp
)

#target_compile_options( sf-xcb-window-test
#  PUBLIC
#  # etc...
#    )

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

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##

add_executable( sf-xcb-window-cairo-test1
    xcb-window-cairo1.cpp
      ../xcb/events.cpp
      ../xcb/xcb.cpp
      ../xcb/window.cpp
      ../cairo/surface.cpp
    ../../hacks/hack_cxa_throw_exception.cpp
  )
target_include_directories( sf-xcb-window-cairo-test1 PUBLIC ${XCB_INCLUDE_DIRS} ${XCB_AUX_INCLUDE_DIRS} ${CAIRO_XCB_INCLUDE_DIRS})
target_link_libraries( sf-xcb-window-cairo-test1 ${Boost_LIBRARIES} ${XCB_LIBRARIES} ${XCB_AUX_LIBRARIES} ${CAIRO_XCB_LIBRARIES} unwind dl pthread)## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##

add_executable( sf-xcb-window-cairo-test2
    xcb-window-cairo2.cpp
      ../xcb/events.cpp
      ../xcb/xcb.cpp
      ../xcb/window.cpp
      ../cairo/surface.cpp
      ../ui/elements.cpp
    ../../hacks/hack_cxa_throw_exception.cpp
  )
target_include_directories( sf-xcb-window-cairo-test2 PUBLIC ${XCB_INCLUDE_DIRS} ${XCB_AUX_INCLUDE_DIRS} ${CAIRO_XCB_INCLUDE_DIRS})
target_link_libraries( sf-xcb-window-cairo-test2 ${Boost_LIBRARIES} ${XCB_LIBRARIES} ${XCB_AUX_LIBRARIES} ${CAIRO_XCB_LIBRARIES} unwind dl pthread)