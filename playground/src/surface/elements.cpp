
#include "fabic/surface/elements.hpp"
#include "fabic/surface/elements-inlines.hpp"


TL_NS_BEGIN
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
  Rectangle<>
  Element::computePositionning(Rectangle<> rect)
  {
    logtrace << "Element::preComputePositionning(" << this->id() << "): begin." ;

    #if 0
    if ( ! this->children().empty() )
    {
      logtrace << " » has " << this->children().size() << " children.";

      for (ElementPtr& elt : this->children()) {
        logtrace << " » Recursing into child \"" << elt->id() << "\" sub-tree.";

        auto child_dimensions = elt->computePositionning(Rectangle());

        logtrace << " » Got child dimensions : "
                 << child_dimensions.width << 'x' << child_dimensions.height;

        self_dimensions.grow_if_bigger(child_dimensions);
      }

      logtrace << " » done with children (of " << this->id() << ')';
    }
    else {
      logtrace << " » is leaf element (no children)." ;
      auto pos = this->computeBoundingBox( rect );
    }

    logtrace << " » Computed '" << this->id() << "' dimensions : "
             << self_dimensions.width << 'x' << self_dimensions.height
             << (this->children().empty() ? "" : " (wrt. children sub-tree).");

    logtrace << "Element::preComputePositionning(" << this->id() << "): end." ;

    this->attributes()->positionning()->dimensions( self_dimensions );

    return self_dimensions;
    #endif // 0

    return rect;
  }

  // virtual btw.
  Rectangle<>
  Element::computeBoundingBox(Rectangle<> rect)
  {
    uint8_t p = this->attributes()->positionning()->placement();

    switch(p) {
    case Placement::BOTTOM:
      break;
    default:
      logerror << "Placement enum. value " << p << " is not handled (switch-case-default)." ;
    }

    return rect;
  }

  // virtual btw.
  void Element::render()
  {
    // Initialize surface _this_ element surface as needed (no replace).
    if (this->parent() != nullptr) {
      this->surface().createSimilar(
          this->parent()->surface(),
          this->attributes()->positionning()->dimensions(),
          false // no replace => keep the same/previous Cairo surface.
      );
    }

    // Recurse render() into children sub-trees :
    if ( ! this->children().empty() ) {
      for (ElementPtr &elt : this->children()) {
        elt->render();
      }
    }

    // TEMP;
    //auto ctx = this->surface().context();

  }


/*
  Element::self_ptr
  Element::initCairo()
  {
    auto geometry = this->get_geometry();

    // https://www.cairographics.org/manual/cairo-XCB-Surfaces.html#cairo-xcb-surface-create
    //   “ Note: If drawable is a Window, then the function cairo_xcb_surface_set_size()
    //     must be called whenever the size of the window changes.
    //     When drawable is a Window containing child windows then drawing to the
    //     created surface will be clipped by those child windows. When the
    //     created surface is used as a source, the contents of the children will
    //     be included. ”
    //     Returns a pointer to the newly created surface. The caller owns the
    //     surface and should call cairo_surface_destroy() when done with it.
    //     This function always returns a valid pointer, but it will return a
    //     pointer to a "nil" surface if an error such as out of memory occurs.
    //     You can use cairo_surface_status() to check for this.

    this->cairoSurface_ = std::shared_ptr<cairo_surface_t>(
        cairo_xcb_surface_create(
            this->xcb_->getXcbConnectionPtr(),
            this->getXid(),
            this->getVisualType(),
            geometry->width,
            geometry->height
        ),
        TLNS::cairo::SurfaceDeleterFunctor()
    );

    return this;
  }
*/

} // xcb ns
TL_NS_END
