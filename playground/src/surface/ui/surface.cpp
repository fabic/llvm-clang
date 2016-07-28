
# include "surface/ui/surface.hpp"

namespace sf {
  namespace ui {

    Surface::Surface(
        shared_ptr< Xcb > xcb_,
        std::shared_ptr< TextBuffers > buffers_
      )
        : xcb::Window( xcb_ ), // parent ctor.
          textBuffers_( buffers_ )
    {
      this->create(800, 450);
    }


    Surface::~Surface()
    { }


    Surface *
      Surface::render()
    {
      parent::render();

      cairo_t * cr = _surface.context().get();

      cairo_select_font_face(cr, "Monospace",
                             CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_NORMAL);

      cairo_font_extents_t font_extents;
      cairo_font_extents(cr, &font_extents);

      cairo_set_font_size(cr, 12);
      cairo_set_source_rgba(cr, 232, 232, 232, 255);

      cairo_move_to(cr, 2, _surface.dimensions().height() - 2);

      cairo_show_text(cr, "Hello world !");


      int lineNb = 0;

      for(auto&& line : textBuffer_()->lines())
      {
        lineNb++;

        logtrace << "Surface::render(): Line #" << lineNb << ' '
                 << line.string() ;
      }

      return this;
    }

  } // ui ns.
} // sf ns.
