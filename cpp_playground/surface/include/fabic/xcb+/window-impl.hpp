#ifndef FABIC_XCB_WINDOW_IMPL_H
#define FABIC_XCB_WINDOW_IMPL_H

#include "fabic/xcb+/window.hpp"

namespace fabic {
namespace xcb {


// todo: create();

inline
Window::Window(xcb_shared_ptr xcb_)
  : Block( nullptr )
  , xcb_( xcb_ )
  , windowXid( 0 ) // fixme: 0 ?
{ }


inline
Window::Window(xcb_shared_ptr xcb_, xcb_window_t xid)
  : Block( nullptr )
  , xcb_( xcb_ )
  , windowXid(xid)
{

  auto geometry = this->get_geometry();

  // https://www.cairographics.org/manual/cairo-XCB-Surfaces.html#cairo-xcb-surface-create
  //   “ Note: If drawable is a Window, then the function cairo_xcb_surface_set_size()
  //     must be called whenever the size of the window changes.
  //     When drawable is a Window containing child windows then drawing to the
  //     created surface will be clipped by those child windows. When the
  //     created surface is used as a source, the contents of the children will
  //     be included. ”
  //     Returns a pointer to the newly created surface. The caller owns the
  //     surface and should call cairo_surface_destroy() when done with it.
  //     This function always returns a valid pointer, but it will return a
  //     pointer to a "nil" surface if an error such as out of memory occurs.
  //     You can use cairo_surface_status() to check for this.

  this->cairoSurface_ = std::shared_ptr<cairo_surface_t>(
    cairo_xcb_surface_create(
        this->xcb_->getXcbConnectionPtr(),
        this->getXid(),
        this->getVisualType(),
        geometry->width,
        geometry->height
      ),
      fabic::cairo::SurfaceDeleterFunctor()
    );

}


Window::~Window() { }


inline
xcb_window_t
  Window::getXid()
{
  return this->windowXid;
}


inline
xcb_visualtype_t *
Window::getVisualType()
{
  if (this->visual != nullptr)
    return this->visual;

  this->visual = xcb_aux_get_visualtype(
      this->xcb_->getXcbConnectionPtr(),
      this->xcb_->getScreenNumber(),
      this->get_attributes()->visual // xcb_visualid_t
    );

  return this->visual;
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


inline
std::unique_ptr<xcb_get_window_attributes_reply_t>
  Window::get_attributes()
{
  auto conn_ = this->xcb_->getXcbConnectionPtr();

  auto cookie = xcb_get_window_attributes(conn_, this->getXid());

  auto attrs_ =
    xcb_get_window_attributes_reply(
      conn_,
      cookie,
      NULL // TODO: generic event thing ?
      );

  if (attrs_ == nullptr)
    throw Xcb::base_exception();

  return std::unique_ptr<xcb_get_window_attributes_reply_t>(attrs_);
}


inline
std::unique_ptr<xcb_get_geometry_reply_t>
  Window::get_geometry()
{
  auto conn_ = this->xcb_->getXcbConnectionPtr();

  auto drawable = this->getXid();

  auto cookie = xcb_get_geometry(conn_, drawable);

  xcb_generic_error_t *error_ = nullptr;

  auto geom_ =
    xcb_get_geometry_reply(
        conn_,
        cookie,
        &error_
      );

  if (geom_ == nullptr)
    throw Xcb::base_exception();

  assert( error_ == nullptr );

  return std::unique_ptr<xcb_get_geometry_reply_t>(geom_);
}


tk::ElementList
  Window::preComputePositionning(
    int16_t w, int16_t h,
    int16_t x, int16_t y
  )
{

}


// // // // // // // // // // // // // // // // // // // // // // // //
// TODO: drop these statics...

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
