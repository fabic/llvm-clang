/** symfony.cpp
 *
 * FabiC.2016-06-23
 */

#include <iostream>
#include <boost/program_options.hpp>

#include "fabic/di/container.hpp"
#include "fabic/di/dll/dll_service_provider.hpp"

namespace fabic {
  namespace symfony {

    namespace po   = boost::program_options;

    using program_arguments = po::variables_map;

    /**
     * MAIN secondary entry point
     *
     * See C-style `int main(argc, argv)` at bottom of file.
     */
    int main(program_arguments &args)
    {
      loginfo << "MAIN: Hello world, you reached this point, can't believe it!";
      loginfo << "  » address : " << args["address"].as<std::string>();
      loginfo << "  » port : " << args["port"].as<int>();
      loginfo << "";

      namespace di = fabic::di;

      auto cnt = di::service_container::new_container_instance();

      // DLL

      auto dll = std::make_shared<di::dll::dll_service_provider>(cnt);

      // dll->load_library("build/symfony/module/http/libmodule-http-server.so");
      // dll->load_library("symfony/module/http/libmodule-http-server.so");
      // dll->load_library("libmodule-http-server.so");
      dll->load_modules_from_self();

      cnt->debugDumpContainer();

      //auto huh = cnt->get_service<server>("http.server");

      cnt->start_startable_services();

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
 * MAIN (the libc) !
 */
int main(int argc, const char *argv[]) {
  auto args = fabic::symfony::process_program_arguments(argc, argv);

  auto exit_status = fabic::symfony::main(args);

  return exit_status;
}
