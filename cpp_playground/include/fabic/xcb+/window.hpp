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

public:

  explicit Window(xcb_shared_ptr xcb_);

  // fixme: drop this.
  Window(xcb_shared_ptr xcb_, xcb_window_t xid);

  virtual ~Window();

  /// Get the associated Xcb connection (shared ptr).
  inline xcb_shared_ptr xcb() { return this->xcb_; }

  /**
   * @return this window's XID.
   */
  xcb_window_t   getXid() const;
  xcb_drawable_t getDrawableXid() const;
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

  virtual void handleEvent(const Event& event);

  virtual void handleEventExpose(
      uint16_t width,
      uint16_t height,
      uint16_t x,
      uint16_t y
    );

  tk::pixels_dimensions_t
    preComputePositionning(
      tk::pixels_dimensions_t dimensions,
      tk::pixels_position_t   position
    ) override;

  void render() override;

protected:
  self_ptr initCairo();
};

} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_WINDOW_H
