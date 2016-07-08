#ifndef FABIC_XCB_TYPEDEFS_H
#define FABIC_XCB_TYPEDEFS_H

#include <cassert>
#include <exception>
#include <map>
#include <memory>
#include <xcb/xcb.h>

# include "fabic/logging.hpp"


namespace fabic {
namespace xcb {

  /**
   * @see Xcb::generate_id().
   */
  typedef uint32_t xid_t;

  /**
   * Convenience typedef for readability.
   */
  typedef xid_t window_xid_t;

  class Window;
  typedef std::shared_ptr< Window > window_shared_ptr;

} // xcb ns.
} // fabic ns.


#endif // FABIC_XCB_TYPEDEFS_H
