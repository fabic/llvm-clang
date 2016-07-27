
# include "surface/ui/surface.hpp"

namespace sf {
  namespace ui {

     using Xcb = xcb::Xcb;
     using XcbWindow = xcb::Window;

    Surface::Surface(shared_ptr< Xcb > xcb_)
        : XcbWindow( xcb_ )
    {
      this->create(800, 450);
    }


    Surface::~Surface()
    { }


  } // ui ns.
} // sf ns.
