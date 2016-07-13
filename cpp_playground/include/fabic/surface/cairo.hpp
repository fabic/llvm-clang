#ifndef FABIC_CAIRO_H
#define FABIC_CAIRO_H

#include <memory>
#include <cairo/cairo-xcb.h>
#include "fabic/xcb+/maths_2d.hpp"
#include "fabic/logging.hpp"

TL_NS_BEGIN
namespace cairo {


using std::shared_ptr;


/**
 * https://www.cairographics.org/manual/cairo-cairo-surface-t.html#cairo-surface-destroy
 * http://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
 */
class SurfaceDeleterFunctor {
public:
  void operator()(cairo_surface_t * surface_) noexcept
  {
    // void
    cairo_surface_destroy(surface_);
    logtrace << "Deleted surface " << surface_;
  }
};


/**
 * Wrapper around Cairo C-style stuff.
 */
class Surface
{
public:
  typedef Surface& self_ref;

private:
  std::shared_ptr< cairo_surface_t > cairoSurface_ = nullptr;

public:


  /**
   * Prototype method for creating _self instances_ for XCB-based
   * Cairo surfaces.
   *
   * @return
   */
   self_ref init_xcb_surface(
      xcb_connection_t *      conn,
      xcb_drawable_t          drawable,
      xcb_visualtype_t *      visual,
      tk::pixels_dimensions_t dimensions
  );
};


} // cairo ns
TL_NS_END

#endif //FABIC_CAIRO_H
