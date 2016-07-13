#ifndef FABIC_XCB_WINDOW_INLINES_H
#define FABIC_XCB_WINDOW_INLINES_H

#include "fabic/xcb+/window.hpp"

namespace fabic {
namespace xcb {


inline xcb_window_t
  Window::getXid() const
{
  return this->windowXid;
}

inline xcb_drawable_t
  Window::getDrawableXid() const
{
  return this->getXid();
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


} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_WINDOW_INLINES_H
