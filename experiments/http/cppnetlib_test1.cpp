

//[ fabic_server_main libs/network/example/http/fabic_server.cpp

/*
  This is a part of the 'Hello World' example. It's used to
  demonstrate how easy it is to set up an HTTP server.  All we do in
  this example is create a request handler and run the server.
        // Copyright 2009 (c) Tarro, Inc.
        // Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
        // Distributed under the Boost Software License, Version 1.0.
        // (See accompanying file LICENSE_1_0.txt or copy at
        // http://www.boost.org/LICENSE_1_0.txt)

  FABIC: copy-pasted from `libs/network/example/http/hello_world_server.cpp` [2016-06-14]

  https://github.com/fabic/cpp-netlib [FORK]
 */
#include <boost/network/protocol/http/server.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <boost/bind/arg.hpp>

    namespace plays {

        namespace http = boost::network::http;
        namespace po   = boost::program_options;

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
         * MAIN
         */
        int main(program_arguments& args) {

            try {
                /*<< Creates the request handler. >>*/
                hello_world handler;

                /*<< Creates the server. >>*/
                server::options options(handler);

                options.address(args["address"].as<std::string>())
                        .port(args["port"].as<std::string>());

                server server_( options );

                /*<< Runs the server. >>*/
                server_.run();
            }
            catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                return 1;
            }

            return 0;
        }

    } // plays ns.


/**
 * C-style main() actual entry point.
 */
int main(int argc, const char *const argv[]) {

    namespace po = boost::program_options;

    // This is needed so for setting up the
    // “ conversion facet according to the user's selected locale ”
    // based on the LANG environment variable. Else an exception is
    // thrown from some deep abyss "character conversion failed".
    //   http://www.boost.org/doc/libs/1_61_0/doc/html/program_options/howto.html#idp308994400
    std::locale::global(std::locale(""));

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("address", po::value<char>(), "Hostname or IP address")
            ("port", po::value<char>(), "TCP port number");

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

    auto exit_status = plays::main(args);

    return exit_status;
}

//]
