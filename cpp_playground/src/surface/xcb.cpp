
#include "fabic/surface/xcb.hpp"
#include "fabic/surface/xcb-inlines.hpp"
#include "fabic/surface/window-inlines.hpp"

TL_NS_BEGIN
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


Xcb::self
  Xcb::registerWindow(window_shared_ptr win_)
    throw(xcb_window_already_registered)
{
  auto window_xid = win_->getXid();

  auto retv =
    this->windows.emplace(
      std::make_pair(
        window_xid,
        win_
        )
    );

  windows_map_t::iterator it = retv.first;
  bool fail                  = retv.second != true;

  if (fail) {
    auto existing_ = it->second;
    throw xcb_window_already_registered( existing_ );
  }

  logdebug << "Xcb::registerWindow(): Window XID " << window_xid << ", ok." ;

  return *this;
}


window_shared_ptr
Xcb::lookupWindow(xcb_window_t window_xid)
{
  auto it = this->windows.find( window_xid );

  bool found = it != this->windows.end();
  if (!found)
    return nullptr;

  return it->second;
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
    | XCB_EVENT_MASK_VISIBILITY_CHANGE
    | XCB_EVENT_MASK_FOCUS_CHANGE
    | XCB_EVENT_MASK_RESIZE_REDIRECT
    | XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW
    | XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE
    | XCB_EVENT_MASK_BUTTON_PRESS   | XCB_EVENT_MASK_BUTTON_RELEASE
    | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_POINTER_MOTION_HINT
    | XCB_EVENT_MASK_BUTTON_MOTION
    | XCB_EVENT_MASK_OWNER_GRAB_BUTTON
    | XCB_EVENT_MASK_KEYMAP_STATE
    | XCB_EVENT_MASK_STRUCTURE_NOTIFY
    | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
    | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
    | XCB_EVENT_MASK_PROPERTY_CHANGE
    | XCB_EVENT_MASK_COLOR_MAP_CHANGE
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

    namespace tk = TLNS::tk;

    auto win_ = tk::Element::_construct< Window >(
        this->shared_from_this(),
        wid
      );

    this->registerWindow( win_ );

    Xcb::assert_void_cookie( _cookie );

  return win_;
}


Xcb_ref_t
  Xcb::run()
{
  logdebug << "Xcb::run() : begin.";

  this->flush();

  Event event( this-> shared_from_this() );

  while( event.next() == true )
  {
    #ifndef NDEBUG
    if ( true ) {
      auto descr = event.description();

      logtrace << "Xcb::run(): Event " << descr.name()
               << " (" << static_cast<int>(descr.type()) << ")"
               << " is-a: " << (descr.struct_name() != nullptr ? descr.struct_name() : "???")
               ;
    }
    #endif // NDEBUG

    this->_handleEvent(event);

  } // end of iteration waiting for events.

  logdebug << "Xcb::run() : end.";

  return *this;
}


void
Xcb::_handleEvent(const Event& event)
{
    xcb_window_t window_xid = event.window_xid();

    logtrace << "  » Window XID: " << window_xid;

    if (window_xid == 0) {
      logdebug << "  » Got no window X ID for this event, ignoring it.";
      return;
    }

    this->_dispatchEvent(event, window_xid);
}


void
Xcb::_dispatchEvent(
    const Event& event,
    xcb_window_t target_window_xid
  )
{
  auto win_ = this->lookupWindow( target_window_xid );

  if (win_ == nullptr) {
    logtrace << "Xcb::_dispatchEvent(): no such window with X ID: "
             << target_window_xid
             << " (ignoring)." ;
    return;
  }

  win_->handleEvent( event );
}


} // xcb ns.
TL_NS_END
