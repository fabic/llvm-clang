
#include <cassert>
# include "fabic/surface/cairo/cairo.hpp"
# include "fabic/logging.hpp"

namespace sf {
  namespace cairo {


    // static btw.
    Surface::self_ref
      Surface::initXCB(
          xcb_connection_t * conn,
          xcb_drawable_t     drawable,
          xcb_visualtype_t * visual,
          Dimensions<>       dimensions
      )
    {
      // https://www.cairographics.org/manual/cairo-XCB-Surfaces.html#cairo-xcb-surface-create
      //
      //   “ Note: If `drawable` is a Window, then the function `cairo_xcb_surface_set_size()`
      //     must be called whenever the size of the window changes.
      //     When `drawable` is a Window containing child windows then drawing to the
      //     created surface will be clipped by those child windows. When the
      //     created surface is used as a source, the contents of the children will
      //     be included. [...]
      //
      //     Returns a pointer to the newly created surface. The caller owns the
      //     surface and should call `cairo_surface_destroy()` when done with it.

      this->cairoSurface_ = std::shared_ptr< cairo_surface_t >(
          cairo_xcb_surface_create(
              conn,
              drawable,
              visual,
              dimensions.width(),
              dimensions.height()
          ),
          // Custom deleter functor.
          SurfaceDeleterFunctor()
      );

      // ^ “ This function `cairo_xcb_surface_create(...)` always returns a valid
      //     pointer, but it will return a pointer to a "nil" surface if an error
      //     such as out of memory occurs. You can use `cairo_surface_status()` to
      //     check for this. [...] ”
      assert( this->cairoSurface_ != nullptr );

      cairo_status_t status = cairo_surface_status( this->cairoSurface_.get() );

      if (status != CAIRO_STATUS_SUCCESS) {
        throw base_exception();
      }

      logtrace << "Surface::initXCB(): "
                  "Cairo XCB surface created, dimensions WxH : "
               << dimensions.width() << 'x' << dimensions.height() ;

      return *this;
    }


    Surface::self_ref
      Surface::createSimilar(
        Surface&      other,
        Dimensions<>  dimensions,
        bool          replace
      )
    {
      if (!replace && this->cairoSurface_ != nullptr)
        return *this;

      assert( replace || this->cairoSurface_ == nullptr );

      logtrace << "Surface::createSimilar(): "
                  "Creating similar Cairo surface with dimensions WxH : "
               << dimensions.width() << 'x' << dimensions.height() ;

      this->cairoSurface_ = std::shared_ptr< cairo_surface_t >(
          cairo_surface_create_similar(
              other.cairoSurface_.get(), // other
              CAIRO_CONTENT_COLOR_ALPHA, // content (https://www.cairographics.org/manual/cairo-cairo-surface-t.html#cairo-content-t)
              dimensions.width(),
              dimensions.height()
          ),
          // Custom deleter functor.
          SurfaceDeleterFunctor()
        );

      cairo_status_t status = cairo_surface_status( this->cairoSurface_.get() );

      if (status != CAIRO_STATUS_SUCCESS) {
        throw base_exception();
      }

      return *this;
    }


    std::shared_ptr< cairo_t >
      Surface::context(bool replace)
    {
      if (!replace && this->cairoContext_ != nullptr)
        return this->cairoContext_;

      assert( replace || this->cairoSurface_ == nullptr );

      this->cairoContext_ = std::shared_ptr< cairo_t >(
          cairo_create( this->cairoSurface_.get() ),
          ContextDeleterFunctor()
      );

      // https://www.cairographics.org/manual/cairo-cairo-t.html#cairo-create
      //   “ This function never returns NULL. ”
      assert( this->cairoContext_ != nullptr );

      cairo_status_t status = cairo_status( this->cairoContext_.get() );

      // https://www.cairographics.org/manual/cairo-Error-handling.html#cairo-status-t
      if (status != CAIRO_STATUS_SUCCESS) {
        throw base_exception();
      }

      return this->cairoContext_;
    }

  } // cairo ns.
} // sf ns.
