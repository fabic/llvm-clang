
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
      return this;
    }

  } // ui ns.
} // sf ns.
