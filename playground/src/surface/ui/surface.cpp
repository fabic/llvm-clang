
# include "surface/ui/surface.hpp"

namespace sf {
  namespace ui {

    Surface::Surface(shared_ptr< Xcb > xcb_)
        : XcbWindow( xcb_ )
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
