
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
  pixels_dimensions_t
  Element::preComputePositionning(
      int16_t width, int16_t height,
      int16_t x, int16_t y
    )
  {
    logtrace << "Element::preComputePositionning(" << this->id() << "): begin." ;

    pixels_dimensions_t
      self_dimensions(
        this->attributes()->positionning()->dimensions(),
        pixels_dimensions_t(width, height)
      );

    if ( ! this->children().empty() ) {
      logtrace << " » has " << this->children().size() << " children.";
      for (tk::ElementPtr &elt : this->children()) {
        logtrace << " » Recursing into child \"" << elt->id() << "\" sub-tree.";

        auto child_dimensions = elt->preComputePositionning(width, height, x, y);

        logtrace << " » Got child dimensions : "
                 << child_dimensions.width << 'x' << child_dimensions.height;

        self_dimensions.grow_if_bigger(child_dimensions);
      }
    }

    logtrace << " » Computed '" << this->id() << "' dimensions : "
             << self_dimensions.width << 'x' << self_dimensions.height
             << (this->children().empty() ? "" : " (wrt. children sub-tree).");

    logtrace << "Element::preComputePositionning(" << this->id() << "): end." ;

    return self_dimensions;
  }

} // xcb ns
} // fabic ns
