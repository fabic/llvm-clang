
#include "fabic/xcb+/elements.hpp"
#include "fabic/xcb+/elements-inline.hpp"


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

} // xcb ns
} // fabic ns
