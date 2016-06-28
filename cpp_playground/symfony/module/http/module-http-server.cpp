
# include "fabic/module/http/module-http-server.hpp"

namespace di = fabic::di;

namespace fabic {
  namespace module {
    namespace http {


      // Ctor.
      http_server::http_server() : options_(*this)
      {
        // set io_serv. ?
        this->server_ = std::make_shared< netlib_http_server_t >( this->options_ );
      }


      // static btw.
      void
      http_server::__di_register_services(di::container_shared_ptr_t container)
      {
        logtrace << "HEY! that's module-http-server __di_register_services() ;- Hura!";

        typedef di::definition<http_server> server_service_t;

        auto http_server_service = std::make_shared<server_service_t>("http.server");

        // Declare the FACTORY functor :
        http_server_service->set_factory_function(
            [](di::base_definition::dependencies_map_ref deps) -> http_server_ptr_t {
              logtrace << "YEAH! that's service `http.server` factory functor bein' invoqued !"
                    " (which is quite remarkable, actually)";

              try {
                http_server_ptr_t server_ = std::make_shared<http_server>();

                /*<< Runs the server. >>*/
                //server_.run();

                return server_;
              }
              catch (std::exception &ex) {
                logfatal << "Caught exception at `http.server` service factory lambda (forwarding it).";
                throw ex;
              }
            }
        );

        // Declare a STARTER functor :
        http_server_service->set_starter_function(
            [](server_service_t::pointer serv) -> bool {
              logtrace << "YEAH! that's service `http.server` factory FUNCTOR bein' invoqued !"
                          " (which is also remarkable)." ;

              return false;
            }
        );

        // cnt->register_service( http_server_service );

        // //http_server_service->requires<SomeClassB>("world");

      }


      void
      http_server::operator()(
          netlib_http_server_t::request const &request,
          netlib_http_server_t::connection_ptr connection
      ) {
        netlib_http_server_t::string_type ip = source(request);
        unsigned int port = request.source_port;

        std::ostringstream data;

        data << "Hello, " << ip << ':' << port << '!';

        connection->write(data.str());
      }

    }
  }
}


// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_dll/tutorial.html#boost_dll.tutorial.searching_for_a_symbol_in_multiple_plugins
BOOST_DLL_ALIAS(
    fabic::module::http::http_server::__di_register_services,
    di_register_services
)
