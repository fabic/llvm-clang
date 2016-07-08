#ifndef FABIC_XCB_WINDOW_H
#define FABIC_XCB_WINDOW_H

#include "fabic/xcb+/xcb.hpp"

namespace fabic {
namespace xcb {

/**
 *
 */
class Window
  : std::enable_shared_from_this<Window>
{
public:
  typedef Window& self;


// Prevent inadvertent copies.
private:
  Window(const Window &) = delete;
  Window& operator=(const Window &) = delete;

protected:
  Xcb& xcb;

  xcb_window_t      windowId;
  xcb_void_cookie_t windowCookie;

public:
  Window(Xcb& xcb)
    : xcb(xcb)
    // , windowCookie()
  { }

  virtual ~Window() { }

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
  self
  create(
    Window&  parentWindow,
    uint16_t width  = 320,
    uint16_t height = 240,
    int16_t  x      = 0,
    int16_t  y      = 0,
    uint16_t borderWidth = 10,
    uint16_t windowClass = XCB_WINDOW_CLASS_INPUT_OUTPUT,
    uint32_t         value_mask = 0,
    const uint32_t * value_list = nullptr)
  {
  }

  self
  createRootedWindow()
  {
    return *this;
  }

};

} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_WINDOW_H
