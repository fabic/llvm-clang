
# include "fabic/surface/surface.hpp"

TL_NS_BEGIN
namespace sf {

namespace xcb = TLNS::xcb;

  Surface::Surface(xcb::xcb_shared_ptr xcb_)
    : Window( xcb_ )
  {
    //this->id("surface");
    this->create(800, 450);
    //this->_initChildrenElementsHierarchy();
  }


  Surface::~Surface()
  { }


} // sf ns.
TL_NS_END
