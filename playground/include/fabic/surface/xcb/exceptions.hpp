#ifndef SURFACE_XCB_EXCEPTIONS_H
#define SURFACE_XCB_EXCEPTIONS_H

#include <memory>
#include "fabic/surface/common.hpp"

namespace sf {
  namespace xcb {

    /// Base exception class for Xcb...
    struct base_exception : sf::ex::base_exception
    {  };

    class Window;

    struct window_already_registered_ex : base_exception
    {
      std::shared_ptr< Window > existing_window_;

      window_already_registered_ex(std::shared_ptr< Window > existing_window)
          : existing_window_(existing_window)
      { }
    };

    struct event_type_mismatch_ex : base_exception {};
    struct unexpected_return_value_from_xcb : base_exception {};
    struct xcb_connection_in_error_condition : base_exception {};
    struct got_nullptr_from_xcb_ex : base_exception {};

  } // xcb ns.
} // sf ns.

#endif // SURFACE_XCB_EXCEPTIONS_H
