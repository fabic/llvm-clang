
# include "fabic/xcb+/cairo.hpp"

namespace fabic {
namespace cairo {


// static btw.
Surface::self_ref
Surface::init_xcb_surface(
    xcb_connection_t *      conn,
    xcb_drawable_t          drawable,
    xcb_visualtype_t *      visual,
    tk::pixels_dimensions_t dimensions
  )
{
  // https://www.cairographics.org/manual/cairo-XCB-Surfaces.html#cairo-xcb-surface-create
  //
  //   “ Note: If drawable is a Window, then the function cairo_xcb_surface_set_size()
  //     must be called whenever the size of the window changes.
  //     When drawable is a Window containing child windows then drawing to the
  //     created surface will be clipped by those child windows. When the
  //     created surface is used as a source, the contents of the children will
  //     be included. [...]
  //
  //     Returns a pointer to the newly created surface. The caller owns the
  //     surface and should call cairo_surface_destroy() when done with it.
  //
  //     This function always returns a valid pointer, but it will return a
  //     pointer to a "nil" surface if an error such as out of memory occurs.
  //     You can use cairo_surface_status() to check for this. [...] ”

  this->cairoSurface_ = std::shared_ptr< cairo_surface_t >(
      cairo_xcb_surface_create(
          conn,
          drawable,
          visual,
          dimensions.width,
          dimensions.height
      ),
      // Custom deleter functor.
      SurfaceDeleterFunctor()
  );

  return *this;
}


} // cairo ns
} // fabic ns
