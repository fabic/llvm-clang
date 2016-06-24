#include "fabic/di/dll/dll_service_provider.hpp"


namespace fabic {
  namespace di   {
    namespace dll {

      void
      dll_service_provider::load_library(boost::filesystem::path path)
      {
          logtrace << "load_library(" << path << ") : begin." ;

          auto lib = boost::dll::shared_library(
              path,
              // Will try/search for .so/.ddl extension,
              // then fallback to the actual `path` as file name.
              //   http://www.boost.org/doc/libs/1_61_0/doc/html/boost/dll/load_mode/type.html
              boost::dll::load_mode::append_decorations
          );

          if ( ! lib.has("create_plugin") ) {
              logwarn << " » skipping library " << path
                  << ": doesn't have symbol (function) : " << module_api_symbol ;
          }
          else {
              logtrace << " » ok, found symbol : " << module_api_symbol ;

              boost::dll::import_alias<module_api_func_t>(
                  std::move(lib),
                  module_api_symbol
              );
          }

          logtrace << "load_library(" << path << ") : end." ;
      }

    }
  }
}
