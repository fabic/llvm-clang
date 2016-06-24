#ifndef FABIC_DI_CONTAINER
#define FABIC_DI_CONTAINER

#include <map>
#include <memory>
#include <forward_list>
#include <string>
#include <cstdio>
#include <typeinfo>
#include <type_traits>
#include <functional>
#include <memory>
#include <cassert>

#include <cxxabi.h>
#include <boost/call_traits.hpp>
//#include <boost/dll/import.hpp>

#include "fabic/di/service_definition.hpp"


namespace fabic {
  namespace di {

    using std::string;
    using std::map;
    using std::pair;

    /**
     * Huh! a service container! in C++ ?
     * _yet another one goin' down this road ! why !?_
     *
     *
     * THE DEPENDENCY INJECTION CONTAINER
     *
     *   a.k.a IoC container
     *   a.k.a service container
     *
     */
    class service_container {
    public:
        typedef std::shared_ptr<service_container> pointer;
        typedef typename boost::call_traits<service_container>::reference reference;
        typedef std::shared_ptr<base_definition> service_ptr_t;

        class service_not_found_exception : std::exception {};
        class service_already_exists_exception : std::exception {};

        /**
         * Wrapper around the std::map<>.
         */
        class service_map {
        public:
            typedef map<string, service_container::service_ptr_t> map_t;
            typedef typename boost::call_traits<map_t>::reference map_ref;
            typedef typename boost::call_traits<service_map>::reference reference;
        private:
            map_t services_;
        private:
            // Prevent implicit copies :
            service_map(const service_map&) = delete;
            service_map& operator=(const service_map&) = delete;
        public:
            explicit service_map() {}
            map_ref get_map_impl() { return this->services_; }

            /**
             * Add a service to the map.
             *
             * @param service
             * @return self
             */
            template<typename T, class PointerT = std::shared_ptr<T>>
            reference insert(std::shared_ptr<service<T,PointerT>> service)
            throw(service_already_exists_exception)
            {
                auto pair = this->services_.insert( std::make_pair(service->id(), service) );

                bool success = pair.second == true;
                if (!success)
                    throw new service_already_exists_exception();

                //auto it = pair.first;

                return *this;
            };

            service_ptr_t find(string id) throw(service_not_found_exception);
        };

    private:
        map<string, base_definition * > services;
        service_map services_;

    private:
        // Prevent implicit copies :
        service_container(const service_container&) = delete;
        service_container& operator=(const service_container&) = delete;

        /**
         * Default ctor made private for we require client codes
         * to go through new_container_instance() for we do need
         * to instantiate this on the heap so that we may have
         * a wrapping shared_ptr.
         */
        explicit service_container();

    public:
        /**
         * Static helper that instantiates a new service_container on the heap
         * that is wrapped into a shared_ptr for injection of the “ container ”
         * service.
         */
        static container_shared_ptr_t new_container_instance();

        /**
         * Register (add) a service definition to this container.
         *
         * @param service
         * @return
         */
        template<typename T, class PointerT = std::shared_ptr<T>>
        reference register_service(std::shared_ptr<service<T,PointerT>> service) {
            this->services_.insert(service);
            return *this;
        };

        /**
         * Fetches a service by its identifier.
         *
         * @param id
         * @return a pointer to the actual thing (typically a shared_ptr<T>).
         */
        template<typename T, class PointerT = std::shared_ptr<T>>
        PointerT get_service(string id) {
            logtrace << "service_container::get_service('" << id << "') :"
                        " about to resolve dependencies..." ;

            service_ptr_t serv = this->resolve_service_dependencies(id);

            logtrace << " » ok, found service : " << serv->id()
                      << ", got a "    << serv->get_service_definition_type_name()
                      << ", address: " << format_address_of(serv) ;

            //typedef std::shared_ptr<service<T, PointerT>> concrete_ptr_t;

            auto concrete = std::dynamic_pointer_cast<service<T, PointerT>>( serv );

            if (concrete != nullptr) {
                logtrace << " » service is-a : " << concrete->get_type_info().name() ;
                return concrete->get_instance();
            }
            else {
                throw new std::exception();
            }
        }

        service_ptr_t resolve_service_dependencies(string service_id);

        service_container& loadFromYamlFile(string filename);

        service_container& debugDumpContainer(std::ostream& os);
    };

  } // di ns.
} // fabic ns.

#endif
