//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_TYPEDEFS_HPP
#define FABICCPPPLAYGROUND_TYPEDEFS_HPP

#include <exception>
#include <memory>
#include <boost/config.hpp>
#include <boost/call_traits.hpp>
#include <boost/throw_exception.hpp>

#include "logging.hpp"
#include "object.hpp"

  namespace di {

    using std::shared_ptr;

    // Forward decl.
    class service_container;

    /**
     * The actual service container instance is managed through a shared pointer.
     */
    typedef shared_ptr<service_container> container_shared_ptr_t;

    // Forward decl.
    template<class T, class PointerT>
    class definition;

    /**
     * Type of the _service definition_ of containers.
     */
    typedef definition<service_container, container_shared_ptr_t>
      container_service_definition_t;

    /**
     * The shared pointer type to the _service definition_ ``of service container instances``.
     */
    typedef shared_ptr< container_service_definition_t >
      container_service_definition_ptr_t;

    // For
    class base_definition;
    typedef shared_ptr< base_definition >
      base_definition_shared_ptr_t;

    //
    // EXCEPTIONS
    //

    struct base_exception
      : virtual boost::exception,
        virtual std::exception
    { };

    struct service_not_found_exception      : base_exception {};
    struct service_already_exists_exception : base_exception {};
    struct service_down_cast_failed : base_exception {};

    struct no_defined_factory_functor : base_exception {};
    struct service_already_constructed : base_exception {};
    struct service_has_no_instance : base_exception {};
    // ^ i.e. has not been constructed yet most probably.

  } // di ns.

/* Symbol visibility... relate to Link Time Optimization
 *
 * http://clang.llvm.org/docs/LTOVisibility.html
 *
 * +-----------------------------------------------------------+  +----------------------------------------------------+
 * | main (clang++ -fvisibility=hidden):                       |  | dso.so (clang++ -fvisibility=hidden):              |
 * |                                                           |  |                                                    |
 * |  +-----------------------------------------------------+  |  |  struct __attribute__((visibility("default"))) C { |
 * |  | LTO unit (clang++ -fvisibility=hidden -flto):       |  |  |    virtual void f();                               |
 * |  |                                                     |  |  |  }                                                 |
 * |  |  struct A { ... };                                  |  |  |  void C::f() {}                                    |
 * |  |  struct [[clang::lto_visibility_public]] B { ... }; |  |  |  struct D {                                        |
 * |  |  struct __attribute__((visibility("default"))) C {  |  |  |    virtual void g() = 0;                           |
 * |  |    virtual void f();                                |  |  |  };                                                |
 * |  |  };                                                 |  |  |  struct E : D {                                    |
 * |  |  struct [[clang::lto_visibility_public]] D {        |  |  |    virtual void g() { ... }                        |
 * |  |    virtual void g() = 0;                            |  |  |  };                                                |
 * |  |  };                                                 |  |  |  __attribute__(visibility("default"))) D *mkE() {  |
 * |  |                                                     |  |  |    return new E;                                   |
 * |  +-----------------------------------------------------+  |  |  }                                                 |
 * |                                                           |  |                                                    |
 * |  struct B { ... };                                        |  +----------------------------------------------------+
 * |                                                           |
 * +-----------------------------------------------------------+
 *
 * “ Class A is not defined outside of main‘s LTO unit, so it can have hidden LTO visibility.
 *   This is inferred from the object file visibility specified on the command line.
 *
 *   Class B is defined in main, both inside and outside its LTO unit.
 *   The definition outside the LTO unit has public LTO visibility,
 *   so the definition inside the LTO unit must also have public LTO
 *   visibility in order to avoid an ODR violation.
 *
 *   Class C is defined in both main and dso.so and therefore must have public LTO visibility.
 *   This is correctly inferred from the visibility attribute.
 *
 *   Class D is an abstract base class with a derived class E defined in dso.so.
 *   This is an example of the COM scenario; the definition of D in main‘s LTO unit must have
 *   public LTO visibility in order to be compatible with the definition of D in dso.so, which
 *   is observable by calling the function mkE. ”
 *
 * Found a define for this purpose from Boost
 * `local/boost-1.61.0-clang/include/boost/config/compiler/clang.hpp`
 * which is automatically included from the frontend include file `boost/config.hpp`.
 *
 * Basically for Clang this would issue `__attribute__((__visibility__("default"))`
 */

#define FABIC_SYMBOL_EXPORT BOOST_SYMBOL_EXPORT

#endif //FABICCPPPLAYGROUND_TYPEDEFS_HPP
