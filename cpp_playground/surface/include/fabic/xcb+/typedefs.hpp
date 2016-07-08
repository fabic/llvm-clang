#ifndef FABIC_XCB_TYPEDEFS_H
#define FABIC_XCB_TYPEDEFS_H

#include <cassert>
#include <exception>
#include <map>
#include <memory>
#include <type_traits>

#include <xcb/xcb.h>

# include "fabic/logging.hpp"


namespace fabic {
namespace xcb {

  using std::map;

  /**
   * @see Xcb::generate_id().
   */
  typedef uint32_t xid_t;

  /**
   * Convenience typedef for readability.
   * EDIT: useless: XCB has a typedef `xcb_window_t`, we'll use theirs.
   */
  typedef xid_t window_xid_t;

  /**
   * It turns out XCB's `xcb_window_t` is a `typedef uint32_t xcb_window_t;`
   * _and_ `xcb_create_window(..., xcb_window_t wid, xcb_window_t parent, ...)`
   * expects us to generate the `wid` argument by means of `xcb_generate_id()`
   * which returns a bare `uint32_t` value.
   */
  static_assert(
      std::is_same<xcb_window_t, window_xid_t>::value
    );

  static_assert(
      std::is_same<xcb_visualid_t, xid_t>::value
    );

  class Xcb;
  typedef std::shared_ptr< Xcb > xcb_shared_ptr;

  class Window;
  typedef std::shared_ptr< Window > window_shared_ptr; // fixme: _t suffix ?

  /**
   * Reference to an `xcb_screen_t` for enforcing correct auto type
   * deduction, ex. `auto scr = Xcb::getScreenInfo();`
   * (`scr` variable _is_ an alias, not an implicit copy on your stack).
   */
  typedef xcb_screen_t& screen_ref_t;

  typedef map<xcb_window_t, window_shared_ptr> windows_map_t;

} // xcb ns.
} // fabic ns.


#endif // FABIC_XCB_TYPEDEFS_H
