
# include "fabic/surface/surface.hpp"

namespace fabic {
namespace sf {

namespace tk  = fabic::tk;
namespace xcb = fabic::xcb;

  Surface::Surface(xcb::xcb_shared_ptr xcb_)
    : Window( xcb_ )
  {
    this->id("surface");
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
    // auto bottom = std::make_shared< Block >(
    //     // this->shared_from_base<Surface>()
    //     nullptr
    //   );

    // auto bottom = (new Block( this->shared_from_this() ))->shared_from_base< Block >();

    auto bottom_ = tk::Element::_construct< Block >( this->shared_from_this() );

    bottom_
        ->id("bottom")
        ->attributes()
        ->positionning()
        ->placement( tk::Placement::BOTTOM )
        ->dimensions(-1, 12)
        ;

    this->appendChild( bottom_ );
  }
} // sf ns.
} // fabic ns.
