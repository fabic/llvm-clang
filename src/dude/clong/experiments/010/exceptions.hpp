#ifndef PIMPLIO_PLUGIN_Exceptions_HPP
#define PIMPLIO_PLUGIN_Exceptions_HPP

#include <stdexcept>

namespace clong {

  /**
   * Base exception class.
   *
   * http://en.cppreference.com/w/cpp/error/runtime_error
   */
  class clong_error : public std::runtime_error {
    using base = std::runtime_error ;
  public:
    explicit clong_error( const std::string& what )
      : base( what ) {}

    explicit clong_error( const char* what = "Hey yo!" )
      : base( what ) {}
  };

  /**
   * Reached a piece of code that lacks an implementation or sthg.
   */
  class not_implemented_error : public clong_error {
  };

  /**
   * Things that happen, but shall not.
   */
  class unexpected_that_was_error : public clong_error {
  };

} // clong ns.

#endif // PIMPLIO_PLUGIN_Exceptions_HPP
