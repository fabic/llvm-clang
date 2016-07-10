
# include "fabic/surface/surface.hpp"

namespace fabic {
namespace sf {

namespace tk  = fabic::tk;
namespace xcb = fabic::xcb;

  Surface::Surface(xcb::xcb_shared_ptr xcb_)
    : Window( xcb_ )
  {
    this->create(640, 380);
    this->initCairo();
  }

  Surface::~Surface()
  { }


} // sf ns.
} // fabic ns.
