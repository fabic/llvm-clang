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
      /// Keep track of the surface dimensions.
      Dimensions<> dimensions_;

      std::shared_ptr< cairo_surface_t > cairoSurface_ = nullptr;

      /// https://www.cairographics.org/manual/cairo-cairo-t.html#cairo-create
      std::shared_ptr< cairo_t > cairoContext_         = nullptr;

    public:

      //inline surface() { return this->cairoSurface_.get(); }

      Dimensions<> dimensions() const { return this->dimensions_; }

      inline std::shared_ptr< cairo_t > context();

      /**
       * Create a Cairo context.
       *
       * \link https://www.cairographics.org/manual/cairo-cairo-t.html
       *
       * \param replace
       * \return
       */
      std::shared_ptr< cairo_t > createCairoContext(bool replace = false);

      self_ref fill(rgba<> color);

      inline self_ref source_rgba(rgba<double> color);

      inline self_ref rectangle(Rectangle<> rectangle);

      /**
       * @link https://www.cairographics.org/manual/cairo-cairo-surface-t.html#cairo-surface-create-similar
       *
       * @param other
       * @param dimensions
       * @return *this
       */
      self_ref
        createSimilarAs(
            Surface &other,
            Dimensions<> dimensions,
            bool replace = false
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


    // // // // // // // // // // // // // // // // // // // // // // //


    inline std::shared_ptr< cairo_t >
      Surface::context()
    {
      if (this->cairoContext_ == nullptr)
        this->createCairoContext();
      return this->cairoContext_;
    }


    inline Surface::self_ref
      Surface::source_rgba(rgba<double> color)
    {
      cairo_set_source_rgba(
          this->context().get(),
          color.r, color.g, color.b, color.a
        );

      return *this;
    }


    inline Surface::self_ref
      Surface::rectangle(Rectangle<> rect)
    {
      cairo_rectangle(
          this->context().get(),
          rect.position().x(),
          rect.position().y(),
          rect.dimensions().width(),
          rect.dimensions().height()
        );

      return *this;
    }

  } // cairo ns.
} // sf ns.

#endif // SF_CAIRO_SURFACE_H
