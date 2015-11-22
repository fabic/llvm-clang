#ifndef FABIC_DI_CONTAINER
#define FABIC_DI_CONTAINER

#include <map>
#include <memory>
#include <typeinfo>

namespace fabic {
    namespace di {

        using std::string;
        using std::map;
        using std::pair;

        class Container {
        public:
            typedef pair<string, void *> service_definition;

        protected:
            map<string, service_definition> services;

        public:
            Container();

            template<typename T>
            Container& registerService(std::string service_id, T& instance) {
                string type_name = typeid(instance).name();
                auto p = std::make_pair(type_name, &instance);
                services.insert( std::make_pair(service_id, p) );
                return *this;
            }

            void * get_service(string id);
        };

    }
}

#endif