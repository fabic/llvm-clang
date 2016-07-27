//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_SERVICE_PROVIDER_HPP
#define FABICCPPPLAYGROUND_SERVICE_PROVIDER_HPP

#include "fabic/di/service_definition.hpp"

  namespace di {

    /**
     * Something that provides service definitions
     * to be declared (added) into the given container.
     */
    class FABIC_SYMBOL_EXPORT service_provider {
    private:
        container_shared_ptr_t container_;

    private:
        service_provider() = delete;
        // Prevent implicit copy
        service_provider(const service_provider &) = delete;
        service_provider &
        operator=(const service_provider &) = delete;

    public:
        /**
         * Constructor
         */
        service_provider(container_shared_ptr_t container)
            : container_(container) {}

        virtual ~service_provider() {}

        container_shared_ptr_t get_service_container() {
            return this->container_ ;
        }
    };

  } // di ns.

#endif //FABICCPPPLAYGROUND_SERVICE_PROVIDER_HPP
