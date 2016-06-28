
#include <boost/dll/alias.hpp>
#include <boost/network/protocol/http/server.hpp>

# include "fabic/di/container.hpp"
# include "fabic/module/http/handler_functor.hpp"

namespace fabic {
  namespace module {
    namespace http {

      namespace bhttp = boost::network::http;

      // Forward decl. for the below typedef `bserver_t`.
      class handler_functor;

      /**
       *
       */
      typedef bhttp::server<handler_functor> bserver_t;


      /**
       *
       */
      class handler_functor {
      public:
        /*<< This is the function that handles the incoming request. >>*/
        void operator()(
            bserver_t::request const &request,
            bserver_t::connection_ptr connection
            )
        {
          bserver_t::string_type ip = source(request);
          unsigned int port = request.source_port;

          std::ostringstream data;

          data << "Hello, " << ip << ':' << port << '!';

          connection->write(data.str());
        }
      };


      //class server : public bhttp::server<handler_functor> {
      // ^ not easy to extend this, actually...

      /**
       * Wrapper around the Boost cpp-netlib server thing.
       */
      class http_server {
      private:
        bserver_t bserver_;
      public:

        /**
         *
         */
        static void
        di_register_services(di::container_shared_ptr_t container);

      };

    }
  }
}
