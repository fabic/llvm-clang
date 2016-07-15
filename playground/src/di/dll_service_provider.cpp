
#include "fabic/di/dll_service_provider.hpp"


TL_NS_BEGIN
  namespace di   {
    namespace dll {

      void
      dll_service_provider::load_library(boost::filesystem::path path)
      {
          logtrace << "load_library(" << path << ") : begin." ;

          boost::dll::shared_library lib(
              path,
              // Will try/search for .so/.ddl extension,
              // then fallback to the actual `path` as file name.
              //   http://www.boost.org/doc/libs/1_61_0/doc/html/boost/dll/load_mode/type.html
              boost::dll::load_mode::append_decorations
          );

          if ( ! lib.has(module_api_symbol) ) {
              logwarn << " » skipping library " << path
                  << ": doesn't have symbol (function) : " << module_api_symbol ;
          }
          else {
              logtrace << " » ok, found symbol : " << module_api_symbol ;

              std::function<module_api_func_t>
                di_register_services_func =
                  boost::dll::import_alias<module_api_func_t>(
                      std::move(lib),
                      module_api_symbol
                  );

              logtrace << " » ok, invoking it, beware !" ;

              di_register_services_func( this->get_service_container() );
          }

          logtrace << "load_library(" << path << ") : end." ;
      }


      void
      dll_service_provider::load_modules_from_self()
      {
          logtrace << "load_modules_from_self() : begin." ;

          boost::dll::library_info libinfo( boost::dll::program_location() );

          std::vector<std::string> symbols = libinfo.symbols("Anna");

          boost::dll::shared_library self( boost::dll::program_location() );

          for(const auto& sym : symbols) {
            logtrace << " » Found symbol from section Anna : " << sym;

            std::function<module_api_func_t>
              di_register_services_func =
                self.get_alias<module_api_func_t>( sym );

            logtrace << " » Got symbol " << sym ;
            logtrace << " » Invoking it now, beware !" ;

            di_register_services_func( this->get_service_container() );
          }

          logtrace << "load_modules_from_self() : end." ;
      }

    }
  }
TL_NS_END
