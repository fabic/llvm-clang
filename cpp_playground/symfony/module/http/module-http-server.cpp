
# include "fabic/module/http/module-http-server.hpp"

namespace di = fabic::di;

    // namespace http = boost::network::http;

    // /*<< Defines the server. >>*/
    // struct hello_world;
    // typedef http::server<hello_world> server;


    // /*<< Defines the request handler.  It's a class that defines two
    //      functions, `operator()` and `log()` >>*/
    // struct hello_world {
    //     /*<< This is the function that handles the incoming request. >>*/
    //     void operator()(server::request const &request, server::connection_ptr connection) {
    //         server::string_type ip = source(request);
    //         unsigned int port = request.source_port;
    //         std::ostringstream data;
    //         data << "Hello, " << ip << ':' << port << '!';
    //         connection->write(data.str());
    //     }
    // };


        // // CPP NETLIB

        // typedef di::definition<server> server_service_t;

        // auto http_server_service = std::make_shared<server_service_t>("http.server");

        // http_server_service->set_factory_function(
        //     [](di::base_definition::dependencies_map_ref deps) -> std::shared_ptr<http::server<hello_world>>
        //     {
        //         std::cerr
        //             << std::endl
        //             << "YEAH! that's service `...` factory functor bein' invoqued"
        //                " which is quite remarkable, actually"
        //             << std::endl;

        //         try {
        //             /*<< Creates the request handler. >>*/
        //             hello_world handler;

        //             /*<< Creates the server. >>*/
        //             server::options options(handler);

        //             // options.address(args["address"].as<std::string>())
        //             //         .port(args["port"].as<std::string>());
        //             options.address("localhost")
        //                     .port("1234");

        //             auto server_ = std::make_shared<server>( options );

        //             /*<< Runs the server. >>*/
        //             //server_.run();
        //             return server_;
        //         }
        //         catch (std::exception &e) {
        //             std::cerr << e.what() << std::endl;
        //             //return nullptr;
        //             throw e;
        //         }
        //     }
        // );

        // http_server_service->set_starter_function(
        //     [](server_service_t::pointer serv) -> bool
        //     {
        //         std::cerr
        //             << std::endl
        //             << "YEAH! that's service `...` factory functor bein' invoqued"
        //                " which is quite remarkable, actually"
        //             << std::endl;
        //         return false;
        //     }
        // );

        // cnt->register_service( http_server_service );

        // //http_server_service->requires<SomeClassB>("world");


void
hey_di_register_services(di::container_shared_ptr_t container)
{
  logtrace << "HEY! that's module-http-server ;- Hura!" ;
}


// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_dll/tutorial.html#boost_dll.tutorial.searching_for_a_symbol_in_multiple_plugins
BOOST_DLL_ALIAS(
    hey_di_register_services,
    di_register_services
)
