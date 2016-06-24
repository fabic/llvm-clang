//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_DLL_SERVICE_PROVIDER_HPP
#define FABICCPPPLAYGROUND_DLL_SERVICE_PROVIDER_HPP

#include "fabic/di/service_provider.hpp"

namespace fabic {
  namespace di   {
    namespace dll {

      class dll_service_provider : service_provider {
      public:
        dll_service_provider(container_shared_ptr_t container)
            : service_provider(container) {}
      };

    } // dll ns.
  } // di ns.
} // fabic ns.

#endif //FABICCPPPLAYGROUND_DLL_SERVICE_PROVIDER_HPP
