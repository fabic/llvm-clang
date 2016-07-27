#ifndef SF_UI_SURFACE_H
#define SF_UI_SURFACE_H

#include "surface/xcb/window.hpp"

namespace sf {
  namespace ui {

    using Xcb = xcb::Xcb;
    using XcbWindow = xcb::Window;


    class Surface
        : public XcbWindow
    {
    public:
      Surface(Xcb::shared_ptr xcb_);
      virtual ~Surface() override;

      //virtual void _initChildrenElementsHierarchy() override;

    };

  } // ui ns.
} // sf ns.

#endif // SF_UI_SURFACE_H
