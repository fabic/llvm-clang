#ifndef SF_CAIRO_H
#define SF_CAIRO_H

#include <memory>
#include <cairo/cairo-xcb.h>

#include "fabic/surface/common.hpp"
#include "fabic/surface/maths/rectangle.hpp"
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

    struct base_exception : sf::ex::base_exception {};

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
          Surface&     other,
          Dimensions<> dimensions,
          bool         replace = false
        );

      /**
       * Prototype method for creating _self instances_ for XCB-based
       * Cairo surfaces.
       *
       * @return
       */
      self_ref
        initXCB(
          xcb_connection_t * conn,
          xcb_drawable_t     drawable,
          xcb_visualtype_t * visual,
          Dimensions<>       dimensions
        );
    };



    /**
     * Functor for automatic invoquation of `cairo_surface_destroy(...)`
     * by `std::shared_ptr< cairo_surface_t >`.
     *
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
     * Likewise above...
     */
    class ContextDeleterFunctor {
    public:
      void operator()(cairo_t * context_) noexcept
      {
        cairo_destroy( context_ );
        logtrace << "Deleted cairo context (cairo_t *) " << context_;
      }
    };


  } // cairo ns.
} // sf ns.

#endif // SF_CAIRO_H
