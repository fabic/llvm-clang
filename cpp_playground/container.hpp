#ifndef FABIC_DI_CONTAINER
#define FABIC_DI_CONTAINER

#include <map>
#include <memory>
#include <forward_list>
#include <string>
#include <ostream>
#include <typeinfo>

#include "ServiceDefinition.hpp"

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

            Container& registerService(string service_id, void * instance, string type_name = nullptr);

            void * get_service(string id);

            Container& loadFromYamlFile(string filename);

            Container& debugDumpContainer(std::ostream& os);
        };

    }
}

#endif