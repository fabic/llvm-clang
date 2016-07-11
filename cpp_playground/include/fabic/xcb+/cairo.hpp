#ifndef FABIC_XCB_CAIRO_H
#define FABIC_XCB_CAIRO_H

#include <memory>
#include <cairo/cairo-xcb.h>

namespace fabic {
namespace cairo {


using std::shared_ptr;


/**
 * https://www.cairographics.org/manual/cairo-cairo-surface-t.html#cairo-surface-destroy
 * http://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
 */
class SurfaceDeleterFunctor {
public:
  void operator()(cairo_surface_t * surface_) noexcept
  {
    // void
    cairo_surface_destroy(surface_);
    logtrace << "Deleted surface " << surface_;
  }
};


} // cairo ns
} // fabic ns

#endif //FABIC_XCB_CAIRO_H
