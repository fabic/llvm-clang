
# include "fabic/surface/surface.hpp"

TL_NS_BEGIN
namespace sf {

namespace tk  = TLNS::tk;
namespace xcb = TLNS::xcb;

  Surface::Surface(xcb::xcb_shared_ptr xcb_)
    : Window( xcb_ )
  {
    this->id("surface");
    this->_create(800, 450);
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
    auto left_   = tk::Element::_construct< Block >( this->shared_from_this() );

    bottom_
        ->id("bottom")
        ->attributes()
        ->positionning()
        ->placement( tk::Placement::BOTTOM )
        ->dimensions( tk::pixels_dimensions_t(-1, 12) )
        ;

    this->appendChild( bottom_ );

    left_
        ->id("left")
        ->attributes()
        ->positionning()
        ->placement( tk::Placement::AUTO )
        ->dimensions( tk::pixels_dimensions_t(-1, -1) )
        ;

    this->appendChild( left_ );
  }
} // sf ns.
TL_NS_END
