
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
    this->_initChildrenElementsHierarchy();
  }

  Surface::~Surface()
  { }

  // virtual btw. from Element.
  void
  Surface::_initChildrenElementsHierarchy()
  {
    auto bottom = std::make_shared< Block >(
        this->shared_from_this()
      );

    //this->appendChild( bottom );
  }
} // sf ns.
} // fabic ns.
