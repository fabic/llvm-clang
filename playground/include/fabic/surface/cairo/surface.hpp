#ifndef SF_CAIRO_SURFACE_H
#define SF_CAIRO_SURFACE_H

#include "fabic/surface/cairo/cairo.hpp"

namespace sf {
  namespace cairo {

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

      //self_ref fill();

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


  } // cairo ns.
} // sf ns.

#endif // SF_CAIRO_SURFACE_H
