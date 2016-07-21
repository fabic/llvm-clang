#ifndef SURFACE_XCB_TYPEDEFS_H
#define SURFACE_XCB_TYPEDEFS_H

#include <cassert>
#include <exception>
#include <map>
#include <memory>
#include <type_traits>

#include <xcb/xcb.h>
#include <xcb/xcb_util.h>

namespace sf {
  namespace xcb {

    using std::map;

    /**
     * \see Xcb::generate_id().
     */
    typedef uint32_t xid_t;

    /* It turns out XCB's `xcb_window_t` is a `typedef uint32_t xcb_window_t;`
     * _and_ `xcb_create_window(..., xcb_window_t wid, xcb_window_t parent, ...)`
     * expects us to generate the `wid` argument by means of `xcb_generate_id()`
     * which returns a bare `uint32_t` value. */
    static_assert( std::is_same<xcb_window_t,   xid_t>::value );
    static_assert( std::is_same<xcb_visualid_t, xid_t>::value );
    static_assert( std::is_same<xcb_drawable_t, xid_t>::value );

    class Xcb;
    typedef std::shared_ptr< Xcb > xcb_shared_ptr;

    typedef Xcb& Xcb_ref_t;

    class Window;
    typedef std::shared_ptr< Window > window_shared_ptr; // fixme: _t suffix ?

    typedef Window& Window_ref_t;

    /**
     * Reference to an `xcb_screen_t` for enforcing correct auto type
     * deduction, ex. `auto scr = Xcb::getScreenInfo();`
     * (`scr` variable _is_ an alias, not an implicit copy on your stack).
     */
    typedef xcb_screen_t& screen_ref_t;

  } // xcb ns.
} // sf ns.

#endif // SURFACE_XCB_TYPEDEFS_H
