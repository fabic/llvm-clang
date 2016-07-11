#ifndef FABIC_XCB_WINDOW_H
#define FABIC_XCB_WINDOW_H

#include "fabic/xcb+/xcb.hpp"
#include "fabic/xcb+/cairo.hpp"
#include "fabic/xcb+/elements.hpp"

namespace fabic {
namespace xcb {

namespace tk = fabic::tk;

/**
 * todo: move to tk::
 */
class Window
  : public tk::Block
{
public:
  typedef Window& self;
  typedef Window* self_ptr;

  static constexpr uint16_t default_window_width  = 640;
  static constexpr uint16_t default_window_height = 480;

// Prevent inadvertent copies.
private:
  Window(const Window &) = delete;
  Window& operator=(const Window &) = delete;

protected:
  xcb_shared_ptr xcb_;
  xcb_window_t   windowXid;
  xcb_visualtype_t * visual = nullptr;
  std::shared_ptr<cairo_surface_t> cairoSurface_ = nullptr;

public:

  explicit Window(xcb_shared_ptr xcb_);

  // fixme: drop this.
  Window(xcb_shared_ptr xcb_, xcb_window_t xid);

  virtual ~Window();

  /**
   * @return this window's XID.
   */
  xcb_window_t   getXid();
  xcb_visualid_t getVisualXid();

  self map();

  std::unique_ptr<xcb_get_window_attributes_reply_t>
    get_attributes();

  std::unique_ptr<xcb_get_geometry_reply_t>
    get_geometry();

  xcb_visualtype_t * getVisualType();

  virtual self_ptr
    create(
        uint16_t          width,
        uint16_t          height,
        window_shared_ptr parentWindow = nullptr
      );

  virtual tk::ElementList
    preComputePositionning(
      int16_t w, int16_t h,
      int16_t x, int16_t y
    ) override;

protected:
  self_ptr initCairo();
};

} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_WINDOW_H
