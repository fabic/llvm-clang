#ifndef SF_UI_SURFACE_H
#define SF_UI_SURFACE_H

#include "surface/xcb/window.hpp"
#include "surface/text/TextBuffers.hpp"

namespace sf {
  namespace ui {

    using std::shared_ptr;
    using Xcb = xcb::Xcb;


    /**
     *
     */
    class Surface
        : public xcb::Window
    {
    public:
      typedef Surface * self_ptr;
      typedef xcb::Window parent;

    protected:
      std::shared_ptr< TextBuffers > textBuffers_;

      // currently displayed buffer.

    public:
      Surface(shared_ptr< Xcb > xcb_, std::shared_ptr< TextBuffers > buffers_);

      virtual ~Surface() override;

      virtual self_ptr render() override;
    };

  } // ui ns.
} // sf ns.

#endif // SF_UI_SURFACE_H
