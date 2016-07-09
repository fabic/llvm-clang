#ifndef FABIC_XCB_IMPL_H
#define FABIC_XCB_IMPL_H

#include "fabic/xcb+/xcb.hpp"

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


inline
xcb_connection_t&
  Xcb::getConnection()
{
  if (this->connection_ == nullptr)
    throw base_exception();

  return *this->connection_;
}


inline
xcb_connection_t *
  Xcb::getXcbConnectionPtr()
{
  if (this->connection_ == nullptr)
    throw base_exception();

  return this->connection_;
}


inline
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


inline
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


inline
xid_t
  Xcb::generate_xid()
{
  return xcb_generate_id( this->connection_ );
}


inline
Xcb::ConnectionError
  Xcb::connection_has_error()
{
  int retv = xcb_connection_has_error(this->connection_);

  if (retv > static_cast<int>(ConnectionError::_LAST_ENUM_SENTINEL))
    throw unexpected_return_value_from_xcb();

  return static_cast<ConnectionError>(retv);
}


inline
int
  Xcb::flush()
{
  int status = xcb_flush( this->connection_ );

  if (status <= 0)
    throw base_exception();

  return status;
}


/// private btw.
inline
void Xcb::throw_if_connection_in_error() {
  if (this->connection_has_error() != ConnectionError::CONN_OK)
    throw xcb_connection_in_error_condition();
}


inline
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


inline
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


// static btw.
inline
void
  Xcb::assert_void_cookie(xcb_void_cookie_t cookie)
{
  assert( cookie.sequence > 0 );
}


inline
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
        xcb_expose_event_t *ev = (xcb_expose_event_t *) event;
        break;
      }

      case XCB_BUTTON_PRESS: {
        /* Handle the ButtonPress event type */
        xcb_button_press_event_t *ev = (xcb_button_press_event_t *) event;

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

#endif // FABIC_XCB_IMPL_H
