#ifndef FABIC_XCB_WINDOW_IMPL_H
#define FABIC_XCB_WINDOW_IMPL_H

#include "fabic/xcb+/window.hpp"

namespace fabic {
namespace xcb {


Window::Window(xcb_shared_ptr xcb_)
  : xcb_( xcb_ )
  , windowXid( 0 ) // fixme: 0 ?
{ }


Window::Window(xcb_shared_ptr xcb_, xcb_window_t xid)
  : xcb_( xcb_ )
  , windowXid(xid)
{ }


Window::~Window() { }


inline
xcb_window_t
  Window::getXid()
{
  return this->windowXid;
}


inline
Window_ref_t
  Window::map()
{
  xcb_void_cookie_t _cookie =
    xcb_map_window(
        this->xcb_->getXcbConnectionPtr(),
        this->getXid()
      );

    Xcb::assert_void_cookie( _cookie );

  return *this;
}


template<uint32_t AttributesBitmask>
inline // static btw.
window_shared_ptr
  Window::create(
      xcb_shared_ptr    xcb_,
      window_shared_ptr parentWindow,
      MaskValues<AttributesBitmask> attributes,
      uint16_t          width,
      uint16_t          height,
      int16_t           x,
      int16_t           y,
      uint16_t          borderWidth,
      uint16_t          windowClass,
      xcb_visualid_t    visualXid
    )
{
  auto wid = xcb_->generate_xid();

  uint32_t value_mask = 0;
  uint32_t * value_list = nullptr;

  if (attributes.size() > 0)
  {
    value_mask = attributes.bitmask();
    value_list = attributes.data();
  }

  // See definition at `/usr/include/xcb/xproto.h:5564`
  // https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html#helloworld
  // http://rosettacode.org/wiki/Window_creation/X11#XCB
  xcb_void_cookie_t _cookie =
    xcb_create_window(
        xcb_->getXcbConnectionPtr(),  // xcb_connection_t *c
        XCB_COPY_FROM_PARENT,        // uint8_t          depth
        wid,                         // xcb_window_t     wid
        parentWindow->getXid(),      // xcb_window_t     parent
        x, y,                        // coordinates (int16_t x, int16_t y)
        width, height,               // dimensions  (int16_t w, int16_t h)
        borderWidth,                 // uint16_t         border_width
        windowClass,                 // uint16_t         _class
        visualXid,                   // xcb_visualid_t   visual
        value_mask,                  // uint32_t         value_mask
        value_list                   // const uint32_t   *value_list
      );

    auto win = make_shared< Window >(xcb_, wid);

    Xcb::assert_void_cookie( _cookie );

    // TODO: xcb_->registerWindow...

    return win;
}


template<uint32_t AttributesBitmask>
inline // static btw.
window_shared_ptr
  Window::createRootedWindow(
      xcb_shared_ptr    xcb_,
      MaskValues<AttributesBitmask> attributes,
      uint16_t          width,
      uint16_t          height
    )
{
  auto root = xcb_->getRootWindow();

  auto win = Window::create(
    xcb_,
    root,
    attributes,
    width, height
    );

  return win;
}


// static btw.
inline
window_shared_ptr
  Window::createSimpleWindow(
      xcb_shared_ptr    xcb_,
      uint16_t          width,
      uint16_t          height
    )
{
  auto screen = xcb_->getScreenInfo();

  constexpr uint32_t bitmask =
      XCB_CW_BACK_PIXEL
    | XCB_CW_BORDER_PIXEL
    | XCB_CW_EVENT_MASK;

  MaskValues<bitmask> attributes;

  attributes[ XCB_CW_EVENT_MASK ] =
      XCB_EVENT_MASK_EXPOSURE
    | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW
    | XCB_EVENT_MASK_KEY_PRESS    | XCB_EVENT_MASK_KEY_RELEASE
    | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE
    | XCB_EVENT_MASK_POINTER_MOTION
    ;

  attributes[ XCB_CW_BACK_PIXEL ] = screen.black_pixel;
  attributes[ XCB_CW_BORDER_PIXEL ] = screen.white_pixel;

  auto win = Window::createRootedWindow(
      xcb_,
      attributes
    );

  return win;
}


} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_WINDOW_IMPL_H
