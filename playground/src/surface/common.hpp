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


    /// Basic exception that carry no semantics, but a reason text.
    struct yet_undefined_exception : base_exception
    {
      const char * narratives_ = nullptr;

      /// Ctor, with some reason text.
      explicit yet_undefined_exception(const char * narratives = nullptr)
        : narratives_( narratives != nullptr ? narratives : "???")
      { }

    };

  }
} // sf ns.

#endif //SF_COMMON_HPP
