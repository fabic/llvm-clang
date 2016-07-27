//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_DLL_SERVICE_PROVIDER_HPP
#define FABICCPPPLAYGROUND_DLL_SERVICE_PROVIDER_HPP

#include <boost/dll/alias.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/runtime_symbol_info.hpp>    // for program_location()
#include <boost/dll/library_info.hpp>

# include "di/service_provider.hpp"

  namespace di   {
    namespace dll {

      class dll_service_provider : service_provider {
      public:
          //typedef boost::shared_ptr<my_plugin_api> (pluginapi_create_t)();
          typedef void (module_api_func_t)(container_shared_ptr_t container);

          std::string module_api_symbol = "di_register_services";

      public:
          dll_service_provider(container_shared_ptr_t container)
              : service_provider(container) {}

          /**
           * http://www.boost.org/doc/libs/1_61_0/libs/filesystem/doc/reference.html#class-path
           *
           * @param path Ex. "build/symfony/libmodule-http-server.so"
           */
          void load_library(boost::filesystem::path path);

          void load_modules_from_self();
      };

    } // dll ns.
  } // di ns.

#endif //FABICCPPPLAYGROUND_DLL_SERVICE_PROVIDER_HPP
