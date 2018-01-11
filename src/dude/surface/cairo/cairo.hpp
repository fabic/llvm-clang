#ifndef SF_CAIRO_H
#define SF_CAIRO_H

#include <memory>
#include <cairo/cairo-xcb.h>

#include "surface/common.hpp"
#include "surface/maths/rectangle.hpp"
#include "surface/color.hpp"
#include "util/logging.hpp"

namespace sf {
  namespace cairo {

    struct base_exception : sf::ex::base_exception {};


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
