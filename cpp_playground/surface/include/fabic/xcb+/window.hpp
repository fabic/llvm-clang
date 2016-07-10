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
  : public std::enable_shared_from_this<Window>
  , public tk::Block
{
public:
  typedef Window& self;

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


  virtual tk::ElementList
    preComputePositionning(
      int16_t w, int16_t h,
      int16_t x, int16_t y
    ) override;



  /**
   *
   */
  template<uint32_t AttributesBitmask>
  static window_shared_ptr
    create(
        xcb_shared_ptr    xcb_,
        window_shared_ptr parentWindow,
        MaskValues<AttributesBitmask> attributes,
        // ^ replaces those two :
        // uint32_t          valueMask  = 0,
        // const uint32_t *  valueList  = nullptr
        uint16_t          width  = default_window_width,
        uint16_t          height = default_window_height,
        int16_t           x      = 0,
        int16_t           y      = 0,
        uint16_t          borderWidth = 10,
        uint16_t          windowClass = XCB_WINDOW_CLASS_INPUT_OUTPUT,
        xcb_visualid_t    visualXid   = XCB_COPY_FROM_PARENT
      );

  /**
   *
   */
  template<uint32_t AttributesBitmask>
  static
  window_shared_ptr
    createRootedWindow(
        xcb_shared_ptr    xcb_,
        MaskValues<AttributesBitmask> attributes,
        uint16_t          width  = default_window_width,
        uint16_t          height = default_window_height
      );

  /**
   *
   */
  static window_shared_ptr
    createSimpleWindow(
        xcb_shared_ptr    xcb_,
        uint16_t          width  = default_window_width,
        uint16_t          height = default_window_height
      );

};

} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_WINDOW_H
