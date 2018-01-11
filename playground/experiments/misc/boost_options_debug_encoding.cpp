/**
 * boost_options_debug_encoding.cpp
 *
 * /me tryin' to debug a situation with Boost's program_options library failing
 * to “convert” program arguments (and anyway I don't want any conversion BTW,
 * but it insists on converting char[] to some std::wstring _with the unexpected
 * intent of converting it back to std::string, it appears).
 *
 * EDIT/2016-06-17: -_- the "bug" disappeared magically after I rebuilt Boost.
 * EDIT/2016-06-17: -_- and re-appeared, grrr!
 * EDIT/2016-06-20: Pb doesn't occur if I build against Clang's libcxx/abi.
 *
 * Buid with :
 *   - $CXX -g -O0 -rdynamic -std=c++14 -frtti -lboost_program_options -lunwind -ldl -lbfd -lpthread -o boost_options_debug_encoding boost_options_debug_encoding.cpp
 *
 *   - $CXX -g -O0 -rdynamic -std=c++14 -stdlib=libc++ -frtti \
           -L../local/boost-1.61.0-clang/lib/ \
           -Wl,-rpath=.,-rpath=../local/boost-1.61.0-clang/lib,-rpath=../local/lib \
           -lboost_program_options-mt -lunwind -ldl \
           -o boost_options_debug_encoding \
                boost_options_debug_encoding.cpp
 *
 * FabiC 2016-06-16 https://github.com/fabic/libcsdbg/
 */

#include <boost/program_options.hpp>
#include <iostream>

// #include "hack_cxa_throw_exception.cpp"

    namespace plays {

        namespace po   = boost::program_options;

        using program_arguments = po::variables_map;

        /**
         * MAIN which accepts
         */
        int main(program_arguments &args) {

            std::cout << "Hello world, you reached this point, can't believe it!" << std::endl;
            std::cout << "  » address : " << args["address"].as<std::string>() << std::endl;
            std::cout << "  » port : "    << args["port"].as<int>() << std::endl;

            return 0;
        }


        /**
         * Resort to Boost's program_options lib.
         * for parsing command line arguments.
         *
         * @param  argc [description]
         * @param  argv [description]
         * @return      [description]
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

    } // plays ns.

/**
 * Invoqued by main() within a try-catch
 * (caught exception here is forwarded).
 */
int main_bis(int argc, const char *argv[])
{
    try {
        auto args = plays::process_program_arguments(argc, argv);
        auto exit_status = plays::main(args);
        return exit_status;
    }
    catch (std::exception& e) {
        std::cerr << "OUPS! Caught sthg: " << e.what() << std::endl;
        std::cerr << "      (forwarding exception)" << std::endl;
        throw e;
    }

    return 127;
}


/**
 * C-style main() actual entry point.
 */
int main(int argc, const char *argv[])
{
    // Init. libcsdbg's tracer thing :
    // using namespace csdbg;
    //
    // tracer *iface = tracer::interface();
    // if ( unlikely(iface == NULL) ) {
    //     std::cerr << "FAILED! couldn't initialize libcsdbg's tracer thing." << std::endl;
    //     return 126;
    // }

    try {
        return main_bis(argc, argv);
    }
    catch (std::exception& e) {
        std::cerr << "HEY! Caught an exception : " << e.what() << std::endl;
        std::cerr << "     (forwarding it, will quite probably end up handled by"
                     " std::terminate() somehow...)" << std::endl;
        throw e;
    }

    return 127;
}
