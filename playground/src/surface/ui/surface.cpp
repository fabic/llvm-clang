
# include "fabic/surface/ui/surface.hpp"

namespace sf {
  namespace ui {

    using Xcb = xcb::Xcb;
    using XcbWindow = xcb::Window;

    Surface::Surface(Xcb::shared_ptr xcb_)
        : XcbWindow( xcb_ )
    {
      this->create(800, 450);
    }


    Surface::~Surface()
    { }


  } // ui ns.
} // sf ns.