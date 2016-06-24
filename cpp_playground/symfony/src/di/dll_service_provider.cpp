#include "fabic/di/dll/dll_service_provider.hpp"


namespace fabic {
  namespace di   {
    namespace dll {

      void
      dll_service_provider::load_library(boost::filesystem::path path) {
          logtrace << "load_library(" << path << ") : begin." ;
          logtrace << "load_library(" << path << ") : end." ;
      }

    }
  }
}
