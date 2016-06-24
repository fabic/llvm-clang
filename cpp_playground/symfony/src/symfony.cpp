/** symfony.cpp
 *
 * FabiC.2016-06-23
 */

#include <iostream>

#include "fabic/di/container.hpp"

#include <boost/program_options.hpp>
#include <boost/network/protocol/http/server.hpp>


namespace fabic {
namespace symfony {

    namespace po   = boost::program_options;
    namespace http = boost::network::http;

    using program_arguments = po::variables_map;


    /*<< Defines the server. >>*/
    struct hello_world;
    typedef http::server<hello_world> server;


    /*<< Defines the request handler.  It's a class that defines two
         functions, `operator()` and `log()` >>*/
    struct hello_world {
        /*<< This is the function that handles the incoming request. >>*/
        void operator()(server::request const &request, server::connection_ptr connection) {
            server::string_type ip = source(request);
            unsigned int port = request.source_port;
            std::ostringstream data;
            data << "Hello, " << ip << ':' << port << '!';
            connection->write(data.str());
        }
    };


    /**
     * MAIN secondary entry point
     *
     * See C-style `int main(argc, argv)` at bottom of file.
     */
    int main(program_arguments &args)
    {
        std::cout
            << std::endl
            << "Hello world, you reached this point, can't believe it!" << std::endl
            << "  » address : " << args["address"].as<std::string>()    << std::endl
            << "  » port : "    << args["port"].as<int>()               << std::endl
            << std::endl
            ;

        namespace di = fabic::di;

        auto cnt = di::Container::new_container_instance();

        typedef di::service<server> server_service_t;

        auto http_server_service = std::make_shared<server_service_t>("http.server");

        http_server_service->set_factory_function(
            [](di::base_service::dependencies_map_ref deps) -> std::shared_ptr<http::server<hello_world>>
            {
                std::cerr
                    << std::endl
                    << "YEAH! that's service `...` factory functor bein' invoqued"
                       " which is quite remarkable, actually"
                    << std::endl;

                try {
                    /*<< Creates the request handler. >>*/
                    hello_world handler;

                    /*<< Creates the server. >>*/
                    server::options options(handler);

                    // options.address(args["address"].as<std::string>())
                    //         .port(args["port"].as<std::string>());
                    options.address("localhost")
                            .port("1234");

                    auto server_ = std::make_shared<server>( options );

                    /*<< Runs the server. >>*/
                    //server_.run();
                    return server_;
                }
                catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                    //return nullptr;
                    throw e;
                }
            }
        );

        http_server_service->set_starter_function(
            [](server_service_t::pointer serv) -> bool
            {
                std::cerr
                    << std::endl
                    << "YEAH! that's service `...` factory functor bein' invoqued"
                       " which is quite remarkable, actually"
                    << std::endl;
                return false;
            }
        );

        cnt->register_service( http_server_service );

        //http_server_service->requires<SomeClassB>("world");

        cnt->debugDumpContainer(std::cout);


        auto huh = cnt->get_service<server>("http.server");


        return 0;
    }


    /**
     * Resort to Boost's program_options lib. for parsing command line arguments.
     */
    program_arguments
    process_program_arguments(int argc, const char *const argv[]) {
        namespace po = boost::program_options;

        // This is needed so for setting up the
        // “ conversion facet according to the user's selected locale ”
        // based on the LANG environment variable. Else an exception is
        // thrown from some deep abyss "character conversion failed".
        //   http://www.boost.org/doc/libs/1_61_0/doc/html/program_options/howto.html#idp308994400
        //   http://en.cppreference.com/w/cpp/locale/locale/global
        std::locale::global(std::locale(""));
        //std::locale::global(std::locale("en_US.UTF-8"));
        //std::locale::global(std::locale::classic());

        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("address",
                    po::value<std::string>()->default_value("localhost"),
                    "Hostname or IP address")
                ("port",
                    po::value<int>()->default_value(1234),
                    "TCP port number");

        po::positional_options_description posit;
        posit.add("extra", -1);

        po::variables_map args;

        auto parser = po::command_line_parser(argc, argv);

        parser
                .options(desc)
                .positional(posit);

        auto parsed_options = parser.run();

        po::store(parsed_options, args, true);

        po::notify(args);

        return args;
    }

} // symfony ns.
} // fabic ns


/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
    try {
        auto args = fabic::symfony::process_program_arguments(argc, argv);
        auto exit_status = fabic::symfony::main(args);
        return exit_status;
    }
    catch (std::exception& e) {
        std::cerr << "OUPS! Caught sthg: " << e.what() << std::endl;
        std::cerr << "      (forwarding exception)" << std::endl;
        throw e;
    }

    return 127;
}
