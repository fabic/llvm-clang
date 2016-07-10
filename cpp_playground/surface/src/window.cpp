
#include "fabic/xcb+/window.hpp"
#include "fabic/xcb+/window-inlines.hpp"
#include "fabic/xcb+/xcb-inlines.hpp"

namespace fabic {
namespace xcb {


// todo: create();

Window::Window(xcb_shared_ptr xcb_)
  : Block( nullptr )
  , xcb_( xcb_ )
  , windowXid( 0 ) // fixme: 0 ?
{ }


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


} // xcb ns.
} // fabic ns.
