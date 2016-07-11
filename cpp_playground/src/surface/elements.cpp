
#include "fabic/xcb+/elements.hpp"
#include "fabic/xcb+/elements-inlines.hpp"


namespace fabic {
namespace tk {

  ContainerTrait::ContainerTrait(ElementPtr parent_)
    : parent_( parent_ )
  { }


  ContainerTrait::~ContainerTrait()
  { }


  Element::Element(ElementPtr parent_)
    : ContainerTrait( parent_ )
  { }


  Element::~Element() { }


  Block::Block(ElementPtr parent_)
    : Element( parent_ )
  { }


  Block::~Block()
  { }


  void
  Element::_initChildrenElementsHierarchy()
  { }


  // virtual default impl.
  ElementList Element::preComputePositionning(
      int16_t w, int16_t h,
      int16_t x, int16_t y
    )
  {
    return ElementList();
  }

} // xcb ns
} // fabic ns
