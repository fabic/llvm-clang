#ifndef FABIC_DI_CONTAINER
#define FABIC_DI_CONTAINER

#include <map>
#include <memory>
#include <forward_list>
#include <string>
#include <ostream>
#include <typeinfo>
#include <type_traits>


#include <cxxabi.h>

#include "object.hpp"
#include "ServiceDefinition.hpp"

namespace fabic {
    namespace di {

        using std::string;
        using std::map;
        using std::pair;

        /**
         *
         */
        class type_info {
        private:
            string mangled_name;
            string type_name;
            bool is_object_instance;
        public:
            type_info(const std::type_info& type, bool _unused)
                    : mangled_name(type.name()),
                      type_name(demangle_cxx_type_name(mangled_name.c_str()))
            { }

            /**
             * Templated ctor.
             *
             * @param v
             * @return
             */
            template<typename T>
            type_info(T& v)
                    : mangled_name(typeid(v).name()),
                      type_name(demangle_cxx_type_name(mangled_name.c_str())),
                      is_object_instance(std::is_convertible<T, object>::value)
            { }

            string name() { return this->type_name; }

            /**
             * Demangles the C++ type name through `abi::__cxa_demangle()`.
             */
            static string demangle_cxx_type_name(const char *mangled_name);
        };

        /**
         *
         */
        class base_service_definition {
        protected:
            string id;
        public:
            explicit base_service_definition(string name) : id(name) {}

            virtual ~base_service_definition() {}

            string get_service_id() { return this->id; }

            virtual type_info& get_type_info() throw(std::exception) {
                throw new std::exception();
            }

            string get_sevice_definition_type_name() {
                return type_info::demangle_cxx_type_name(typeid(*this).name());
            }
        };

        template<class T>
        class service_definition : public base_service_definition {
        private:
            type_info type;
        public:
            service_definition(string service_id)
                    : base_service_definition(service_id),
                      type(typeid(T), false) {}

            virtual ~service_definition() {}

            virtual type_info& get_type_info() noexcept { return this->type; }
        };

        /**
         *
         */
        class base_service {
        protected:
            string id;
        public:
            base_service(string name) : id(name) {}
            virtual ~base_service() {}

            string get_service_id() { return this->id; }

            string get_sevice_type_name() {
                return type_info::demangle_cxx_type_name(typeid(*this).name());
            }

            virtual void* get_void_instance_ptr() = 0;
        };

        /**
         * a "type-able" service definition, one for which typeid() is possible upon.
         * todo: eventually refactor as typeable_service_definition ?
         */
        template<class T>
        class service : public base_service {
        private:
            type_info type;
            T& instance;
        public:
            service(string service_id, T& instance)
                    : base_service(service_id),
                      instance(instance),
                      type(instance) { }

            virtual ~service() {}

            T& get_instance() {
                return this->instance;
            }

            virtual void* get_void_instance_ptr() {
                return static_cast<void*>( &this->get_instance() );
            }
        };

        /**
         * Huh! a service container! in C++ ?
         * _yet another one goin' down this road ! why !?_
         */
        class Container {
        public:
            class service_not_found_exception : std::exception {};

        private:
            map<string, base_service * > services;
            map<string, base_service_definition * > service_definitions;

        public:
            Container();

            //Container& registerService(string service_id, void * instance, string type_name = nullptr);

            template<typename T>
            Container& registerService(string service_id, T& instance) {
                auto def = new service<T>(service_id, instance);
                this->services.insert(std::make_pair(service_id, def));
                return *this;
            }

            template<typename T>
            T& get_service(string id) {
                auto it = this->services.find(id);

                if (it == this->services.end())
                    throw new service_not_found_exception();

                auto base_definition = it->second;

                auto typed_definition = dynamic_cast< service<T>* >(base_definition);

                if (typed_definition != nullptr) {
                    return typed_definition->get_instance();
                }
                else {
                    throw new std::exception();
                }
            }

            template<typename T>
            service_definition<T>&
            require(string service_id) {
                auto def = new service_definition<T>(service_id);
                this->service_definitions.insert(std::make_pair(service_id, def));
                return *def;
            }

            Container& loadFromYamlFile(string filename);

            Container& debugDumpContainer(std::ostream& os);
        };

    }
}

#endif