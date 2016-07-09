#ifndef FABIC_XCB_WINDOW_H
#define FABIC_XCB_WINDOW_H

#include "fabic/xcb+/xcb.hpp"
#include "fabic/xcb+/mask_values.hpp"

namespace fabic {
namespace xcb {

/**
 *
 */
class Window
  : public std::enable_shared_from_this<Window>
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

public:

  explicit Window(xcb_shared_ptr xcb_);

  Window(xcb_shared_ptr xcb_, xcb_window_t xid);

  virtual ~Window();

  /**
   * @return this window's XID.
   */
  xcb_window_t getXid();

  /**
   * @brief Makes a window visible
   *
   *    xcb_void_cookie_t
   *    xcb_map_window (xcb_connection_t *c,
   *                    xcb_window_t      window);
   *
   * @param c The connection
   * @param window The window to make visible.
   * @return A cookie
   *
   * Maps the specified window. This means making the window visible (as long as its
   * parent is visible).
   *
   * This MapWindow request will be translated to a MapRequest request if a window
   * manager is running. The window manager then decides to either map the window or
   * not. Set the override-redirect window attribute to true if you want to bypass
   * this mechanism.
   *
   * If the window manager decides to map the window (or if no window manager is
   * running), a MapNotify event is generated.
   *
   * If the window becomes viewable and no earlier contents for it are remembered,
   * the X server tiles the window with its background. If the window's background
   * is undefined, the existing screen contents are not altered, and the X server
   * generates zero or more Expose events.
   *
   * If the window type is InputOutput, an Expose event will be generated when the
   * window becomes visible. The normal response to an Expose event should be to
   * repaint the window.
   *
   */
  self map();

  /**
   *
   * @brief Creates a window -- `xcb_create_window()`
   *
   * @param c The connection
   * @param depth Specifies the new window's depth (TODO: what unit?).
   *
   * The special value `XCB_COPY_FROM_PARENT` means the depth is taken from the
   * \a parent window.
   *
   * @param wid The ID with which you will refer to the new window, created by
   * `xcb_generate_id`.
   *
   * @param parent The parent window of the new window.
   *
   * @param x The X coordinate of the new window.
   * @param y The Y coordinate of the new window.
   *
   * @param width The width of the new window.
   * @param height The height of the new window.
   *
   * @param border_width TODO:
   * \n
   * Must be zero if the `class` is `InputOnly` or a `xcb_match_error_t` occurs.
   *
   * @param _class A bitmask of #xcb_window_class_t values.
   *
   * @param visual Specifies the id for the new window's visual.
   * \n
   * The special value `XCB_COPY_FROM_PARENT` means the visual is taken from the
   * \a parent window.
   *
   * @param value_mask A bitmask of #xcb_cw_t values.
   *
   * @return A cookie
   *
   * Creates an unmapped window as child of the specified \a parent window. A
   * CreateNotify event will be generated. The new window is placed on top in the
   * stacking order with respect to siblings.
   *
   * The coordinate system has the X axis horizontal and the Y axis vertical with
   * the origin [0, 0] at the upper-left corner. Coordinates are integral, in terms
   * of pixels, and coincide with pixel centers. Each window and pixmap has its own
   * coordinate system. For a window, the origin is inside the border at the inside,
   * upper-left corner.
   *
   * The created window is not yet displayed (mapped), call `xcb_map_window` to
   * display it.
   *
   * The created window will initially use the same cursor as its parent.
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
  inline static
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
