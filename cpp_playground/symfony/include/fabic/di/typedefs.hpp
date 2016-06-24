//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_TYPEDEFS_HPP
#define FABICCPPPLAYGROUND_TYPEDEFS_HPP

namespace fabic {
  namespace di {

    // Forward decl.
    class Container;

    // Forward decl.
    template<class T, class PointerT>
    class service;

    /**
     * The actual service container instance is managed through a shared pointer.
     */
    typedef std::shared_ptr <Container> container_shared_ptr_t;


    /**
     * Type of the _service definition_ for containers.
     */
    typedef service<Container, container_shared_ptr_t> container_service_definition_t;

    /**
     * The shared pointer type to the _service definition_ ``of service container instances``.
     */
    typedef std::shared_ptr <container_service_definition_t> container_service_definition_ptr_t;

  } // di ns.
} // fabic ns.

#endif //FABICCPPPLAYGROUND_TYPEDEFS_HPP
