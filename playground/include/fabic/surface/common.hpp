#ifndef SF_COMMON_HPP
#define SF_COMMON_HPP

#include <exception>
# include <boost/exception/exception.hpp>

namespace sf {
  namespace ex {

    /// Base exception class for Xcb...
    struct base_exception
        : virtual std::exception,
          virtual boost::exception
    {  };

  }
} // sf ns.

#endif //SF_COMMON_HPP
