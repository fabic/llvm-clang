
#include <boost/dll/alias.hpp>

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <boost/asio.hpp>

// For boost::asio::signal_set
//   http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/reference/signal_set.html
#include <signal.h>

// For deadline timers
//   http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/tutorial/tuttimer2.html
#include <boost/date_time/posix_time/posix_time.hpp>
// Using the std:: one instead ?
//   http://en.cppreference.com/w/cpp/chrono
// #include <chrono>

# include "di/container.hpp"

  namespace module {
    namespace asio {

      using boost::asio::io_service;

      /**
       * Beware! we're extending Boost's io_service which isn't designed
       * "behaviour sub-typing" (no virtual dispatch).
       */
      class IoService
          : public io_service
      {
      private:
        IoService(const IoService&) = delete;
        IoService& operator=(const IoService &) = delete;
      public:
        /// Ctor
        IoService();

        ~IoService();
      };

    }
  }
