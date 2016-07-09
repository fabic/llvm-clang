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
  explicit Window(xcb_shared_ptr xcb_)
    : xcb_( xcb_ )
    , windowXid( 0 ) // fixme: 0 ?
  { }

  /**
   */
  Window(xcb_shared_ptr xcb_, xcb_window_t xid)
    : xcb_( xcb_ )
    , windowXid(xid)
  { }

  virtual ~Window() { }

  xcb_window_t getXid() {
    return this->windowXid;
  }

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
  self
    map()
  {
    xcb_void_cookie_t _cookie =
      xcb_map_window(
          this->xcb_->getXcbConnectionPtr(),
          this->getXid()
        );

      Xcb::assert_void_cookie( _cookie );

    return *this;
  }

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
  static
  window_shared_ptr
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
      )
  {
    auto wid = xcb_->generate_xid();

    uint32_t value_mask = 0;
    uint32_t * value_list = nullptr;

    if (attributes.size() > 0)
    {
      value_mask = attributes.bitmask();
      value_list = attributes.data();
    }

    // See definition at `/usr/include/xcb/xproto.h:5564`
    // https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html#helloworld
    // http://rosettacode.org/wiki/Window_creation/X11#XCB
    xcb_void_cookie_t _cookie =
      xcb_create_window(
          xcb_->getXcbConnectionPtr(),  // xcb_connection_t *c
          XCB_COPY_FROM_PARENT,        // uint8_t          depth
          wid,                         // xcb_window_t     wid
          parentWindow->getXid(),      // xcb_window_t     parent
          x, y,                        // coordinates (int16_t x, int16_t y)
          width, height,               // dimensions  (int16_t w, int16_t h)
          borderWidth,                 // uint16_t         border_width
          windowClass,                 // uint16_t         _class
          visualXid,                   // xcb_visualid_t   visual
          value_mask,                  // uint32_t         value_mask
          value_list                   // const uint32_t   *value_list
        );

      auto win = make_shared< Window >(xcb_, wid);

      Xcb::assert_void_cookie( _cookie );

      return win;
  }

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
      )
  {
    auto root = xcb_->getRootWindow();

    auto win = Window::create(
      xcb_,
      root,
      attributes,
      width, height
      );

    return win;
  }

  /**
   *
   */
  static window_shared_ptr
    createSimpleWindow(
        xcb_shared_ptr    xcb_,
        uint16_t          width  = default_window_width,
        uint16_t          height = default_window_height
      )
  {
    auto screen = xcb_->getScreenInfo();

    constexpr uint32_t bitmask =
        XCB_CW_BACK_PIXEL
      | XCB_CW_BORDER_PIXEL
      | XCB_CW_EVENT_MASK;

    MaskValues<bitmask> attributes;

    attributes[ XCB_CW_EVENT_MASK ] =
        XCB_EVENT_MASK_EXPOSURE
      | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW
      | XCB_EVENT_MASK_KEY_PRESS    | XCB_EVENT_MASK_KEY_RELEASE
      | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE
      | XCB_EVENT_MASK_POINTER_MOTION
      ;

    attributes[ XCB_CW_BACK_PIXEL ] = screen.black_pixel;
    attributes[ XCB_CW_BORDER_PIXEL ] = screen.white_pixel;

    auto win = Window::createRootedWindow(
        xcb_,
        attributes
      );

    return win;
  }


};

} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_WINDOW_H
