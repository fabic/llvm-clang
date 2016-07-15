#ifndef FABIC_SURFACE_H
#define FABIC_SURFACE_H

#include "fabic/surface/xcb+.hpp"

TL_NS_BEGIN
namespace sf {

namespace tk  = TLNS::tk;
namespace xcb = TLNS::xcb;


class Surface
  : public xcb::Window
{
public:
  Surface(xcb::xcb_shared_ptr xcb_);
  virtual ~Surface() override ;

  virtual void _initChildrenElementsHierarchy() override ;

};


} // sf ns.
TL_NS_END

#endif // FABIC_SURFACE_H
