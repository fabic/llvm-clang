
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
  Element::computePositionning(
      pixels_dimensions_t  dimensions,
      pixels_position_t    position
    )
  {
    pixels_dimensions_t self_dimensions =
        this->preComputePositionning(dimensions, position);

    // todo: impl. sthg here ?

    // todo: postComputePositionning ?

    return self_dimensions;
  }


  // virtual default impl.
  pixels_dimensions_t
  Element::preComputePositionning(
      pixels_dimensions_t  dimensions,
      pixels_position_t    position
    )
  {
    logtrace << "Element::preComputePositionning(" << this->id() << "): begin." ;

    pixels_dimensions_t
      self_dimensions(
        this->attributes()->positionning()->dimensions(),
        dimensions
      );

    if ( ! this->children().empty() ) {
      logtrace << " » has " << this->children().size() << " children.";
      for (tk::ElementPtr &elt : this->children()) {
        logtrace << " » Recursing into child \"" << elt->id() << "\" sub-tree.";

        auto child_dimensions = elt->preComputePositionning(
            self_dimensions,
            position
          );

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


  // virtual btw.
  void Element::render()
  {

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
        fabic::cairo::SurfaceDeleterFunctor()
    );

    return this;
  }
*/

} // xcb ns
} // fabic ns
