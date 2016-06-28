
#include <boost/dll/alias.hpp>
#include <boost/network/protocol/http/server.hpp>

# include "fabic/di/container.hpp"

namespace fabic {
  namespace module {
    namespace http {

      namespace http = boost::network::http;

      class server;
      struct server::handler_functor;

      class server : public http::server<server::handler_functor> {
      public:
        struct handler_functor {
          /*<< This is the function that handles the incoming request. >>*/
          void operator()(
              server::request const &request,
              server::connection_ptr connection)
          {
            server::string_type ip = source(request);
            unsigned int port = request.source_port;
            std::ostringstream data;
            data << "Hello, " << ip << ':' << port << '!';
            connection->write(data.str());
          }
        };


      };

    }
  }
}
