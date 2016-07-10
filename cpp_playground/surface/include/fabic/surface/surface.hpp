#ifndef FABIC_SURFACE_H
#define FABIC_SURFACE_H

#include "fabic/xcb+/xcb+.hpp"

namespace fabic {
namespace sf {

namespace tk  = fabic::tk;
namespace xcb = fabic::xcb;


class Surface
  : public xcb::Window
{
public:
  Surface(xcb::xcb_shared_ptr xcb_);
  virtual ~Surface() override ;
};


} // sf ns.
} // fabic ns.

#endif // FABIC_SURFACE_H
