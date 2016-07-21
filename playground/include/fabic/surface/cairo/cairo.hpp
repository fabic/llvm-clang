#ifndef SF_CAIRO_H
#define SF_CAIRO_H

#include <exception>
#include <memory>
#include <cairo/cairo-xcb.h>

#include "fabic/surface/maths_2d.hpp"
#include "fabic/logging.hpp"

namespace sf {

  // FIXME: TEMP
  namespace color {
    template< typename scalar_type >
    struct rgba {
      scalar_type r = 0;
      scalar_type g = 0;
      scalar_type b = 0;
      scalar_type a = 0;

      inline rgba(scalar_type r, scalar_type g, scalar_type b, scalar_type a)
          : r(r), g(g), b(b), a(a)
      { }
    };
  }


  namespace cairo {

    struct base_exception : std::exception {};


  /**
   * https://www.cairographics.org/manual/cairo-cairo-surface-t.html#cairo-surface-destroy
   * http://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
   */
    class SurfaceDeleterFunctor {
    public:
      void operator()(cairo_surface_t * surface_) noexcept
      {
        cairo_surface_destroy( surface_ );
        logtrace << "Deleted cairo surface (cairo_surface_t *) " << surface_;
      }
    };


    /**
     *
     */
    class ContextDeleterFunctor {
    public:
      void operator()(cairo_t * context_) noexcept
      {
        cairo_destroy( context_ );
        logtrace << "Deleted cairo context (cairo_t *) " << context_;
      }
    };



  /**
   * Wrapper around Cairo C-style stuff.
   */
    class Surface
    {
    public:
      typedef Surface& self_ref;
      typedef Surface& reference;

    private:
      std::shared_ptr< cairo_surface_t > cairoSurface_ = nullptr;

      /// https://www.cairographics.org/manual/cairo-cairo-t.html#cairo-create
      std::shared_ptr< cairo_t > cairoContext_         = nullptr;

    public:

      //inline surface() { return this->cairoSurface_.get(); }

      std::shared_ptr< cairo_t > context(bool replace = false);

      self_ref fill();

      /**
       * @link https://www.cairographics.org/manual/cairo-cairo-surface-t.html#cairo-surface-create-similar
       *
       * @param other
       * @param dimensions
       * @return *this
       */
      self_ref
      createSimilar(
          Surface&                other,
          tk::pixels_dimensions_t dimensions,
          bool                    replace = false
      );

      /**
       * Prototype method for creating _self instances_ for XCB-based
       * Cairo surfaces.
       *
       * @return
       */
      self_ref initXcb(
          xcb_connection_t *conn,
          xcb_drawable_t drawable,
          xcb_visualtype_t *visual,
          tk::pixels_dimensions_t dimensions
      );
    };


  } // cairo ns.
} // sf ns.

#endif // SF_CAIRO_H
