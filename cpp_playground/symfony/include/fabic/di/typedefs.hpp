//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_TYPEDEFS_HPP
#define FABICCPPPLAYGROUND_TYPEDEFS_HPP

#include <memory>
#include <boost/call_traits.hpp>

#include "fabic/logging.hpp"
#include "fabic/object.hpp"

namespace fabic {
  namespace di {

    // Forward decl.
    class service_container;

    // Forward decl.
    template<class T, class PointerT>
    class definition;

    /**
     * The actual service container instance is managed through a shared pointer.
     */
    typedef std::shared_ptr<service_container> container_shared_ptr_t;


    /**
     * Type of the _service definition_ for containers.
     */
    typedef definition<service_container, container_shared_ptr_t> container_service_definition_t;

    /**
     * The shared pointer type to the _service definition_ ``of service container instances``.
     */
    typedef std::shared_ptr <container_service_definition_t> container_service_definition_ptr_t;

  } // di ns.
} // fabic ns.

#endif //FABICCPPPLAYGROUND_TYPEDEFS_HPP
