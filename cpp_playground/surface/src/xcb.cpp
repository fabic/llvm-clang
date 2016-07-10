
#include "fabic/xcb+/xcb.hpp"
#include "fabic/xcb+/xcb-inlines.hpp"
#include "fabic/xcb+/window-inlines.hpp"

namespace fabic {
namespace xcb {


using std::make_shared;
using std::shared_ptr;
using std::make_pair;
using std::pair;



Xcb::Xcb() {
  this->connect();
}


Xcb::~Xcb()
{
  if (this->connection_ != nullptr)
    this->disconnect();
}


Xcb_ref_t
  Xcb::connect(const char *displayName)
{
  logtrace << "Xcb: Connecting to "
           << (displayName ? displayName : "default display.");

  if (this->connection_ != nullptr)
    throw base_exception();

  this->connection_ = xcb_connect(displayName, &this->screenNumber);

  this->throw_if_connection_in_error();

  logtrace << "Xcb: connected (probably).";

  return *this;
}


Xcb_ref_t
  Xcb::disconnect()
{
  logtrace << "Xcb: Disconnecting from server...";

  if (this->connection_ == nullptr)
    throw base_exception();

  xcb_disconnect(this->connection_);

  logtrace << "Xcb: Disconnected.";

  this->connection_ = nullptr;

  return *this;
}


screen_ref_t&
  Xcb::getScreenInfo(int screenNbr)
{
  screenNbr = screenNbr > -1 ?
    screenNbr : this->screenNumber;

  xcb_screen_iterator_t iter =
    xcb_setup_roots_iterator(
        xcb_get_setup( this->connection_ )
      );
  // todo: ^ see if we may have error conditions with these 2 xcb_... functions.

  while( iter.rem > 0 && screenNbr > 0) {
    xcb_screen_next( &iter );
    screenNbr--;
  }

  xcb_screen_t * screen = iter.data;

  assert( screen != nullptr );

  return *screen;
}


window_shared_ptr
  Xcb::getRootWindow()
{
  if (this->rootWindow != nullptr)
    return this->rootWindow;

  xcb_screen_t& screen = this->getScreenInfo();
  xcb_window_t rootXid = screen.root;

  pair<windows_map_t::iterator, bool> retv =
    this->windows.emplace(
        make_pair(rootXid, make_shared< Window >(this->shared_from_this(), rootXid))
      );

  bool failed = retv.second == false;

  if (failed) {
    throw base_exception();
  }

  auto inserted_it = retv.first;

  window_shared_ptr win = inserted_it->second;

  this->rootWindow = win;

  return this->rootWindow;
}


window_shared_ptr
  Xcb::createWindowSimple(
      window_shared_ptr parentWindow,
      uint16_t          width,
      uint16_t          height
    )
{
  if (parentWindow == nullptr)
    parentWindow = this->getRootWindow();

  auto screen = this->getScreenInfo();

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

  attributes[ XCB_CW_BACK_PIXEL ]   = screen.black_pixel;
  attributes[ XCB_CW_BORDER_PIXEL ] = screen.white_pixel;

  uint32_t value_mask  = attributes.bitmask();
  uint32_t* value_list = attributes.data();

  auto wid = this->generate_xid();

  uint16_t       borderWidth = 10;
  uint16_t       windowClass = XCB_WINDOW_CLASS_INPUT_OUTPUT;
  xcb_visualid_t visualXid   = XCB_COPY_FROM_PARENT;

  // See definition at `/usr/include/xcb/xproto.h:5564`
  // https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html#helloworld
  // http://rosettacode.org/wiki/Window_creation/X11#XCB
  xcb_void_cookie_t _cookie =
    xcb_create_window(
        this->getXcbConnectionPtr(),  // xcb_connection_t *c
        XCB_COPY_FROM_PARENT,        // uint8_t          depth
        wid,                         // xcb_window_t     wid
        parentWindow->getXid(),      // xcb_window_t     parent
        0, 0,                        // coordinates (int16_t x, int16_t y)
        width, height,               // dimensions  (int16_t w, int16_t h)
        borderWidth,                 // uint16_t         border_width
        windowClass,                 // uint16_t         _class
        visualXid,                   // xcb_visualid_t   visual
        value_mask,                  // uint32_t         value_mask
        value_list                   // const uint32_t   *value_list
      );

    auto win = make_shared< Window >(
      this->shared_from_this(),
      wid
      );

    Xcb::assert_void_cookie( _cookie );

  return win;
}


Xcb_ref_t
  Xcb::run()
{
  logdebug << "Xcb::run() : begin.";

  this->flush();

  xcb_generic_event_t * event;

  while (nullptr != (event = xcb_wait_for_event( this->connection_ )))
  {
    // todo: What is this masking agaisnt ~0x80 about ?
    auto e = event->response_type & ~0x80; // uint8_t btw.

    switch (e) {
      case XCB_EXPOSE: {
        logtrace << "Xcb::run(): EXPOSE !";
        // xcb_expose_event_t *ev = (xcb_expose_event_t *) event;
        break;
      }

      case XCB_BUTTON_PRESS: {
        /* Handle the ButtonPress event type */
        // xcb_button_press_event_t *ev = (xcb_button_press_event_t *) event;

        /* ... */

        break;
      }

      case XCB_KEY_PRESS: {
        logtrace << "Xcb::run(): KEY PRESSED !";
        break;
      }

      // Unknown event type, ignore it.
      default: {
        logtrace << "Xcb::run(): Event " << e << " is unknown to us.";
        break;
      }
    }

    free( event );
  } // end of iteration waiting for events.

  logdebug << "Xcb::run() : end.";

  return *this;
}


} // xcb ns.
} // fabic ns.
