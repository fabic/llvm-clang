#ifndef SF_SURFACE_H
#define SF_SURFACE_H

#include "fabic/surface/xcb+.hpp"

namespace sf {

  namespace tk  = TLNS::tk;
  namespace xcb = TLNS::xcb;

  class Surface
      : public xcb::Window {
  public:
    Surface(xcb::xcb_shared_ptr xcb_);
    virtual ~Surface() override;

    //virtual void _initChildrenElementsHierarchy() override;

  };

} // sf ns.

#endif // SF_SURFACE_H
