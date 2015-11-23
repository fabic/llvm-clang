//
// Created by fabi on 11/21/15.
//

#include <yaml-cpp/yaml.h>
#include <iostream>
#include "container.hpp"

namespace fabic {
    namespace di {

        Container::Container() {}

		Container& Container::registerService(string service_id, void * instance, string type_name) {
			auto p = std::make_pair(type_name, &instance);
			services.insert( std::make_pair(service_id, p) );
			return *this;
		}

		void * Container::get_service(string id) {
        	auto it = services.find(id);
        	if (it == services.end())
        		return nullptr;
        	return it->second.second;
        }

        Container& Container::loadFromYamlFile(string filename) {
			YAML::Node yaml = YAML::LoadFile(filename);
            std::cerr << yaml.Type() << std::endl;
            const auto& srvs = yaml["services"];
            if (srvs.IsDefined()) {
                for(auto it = srvs.begin(); it != srvs.end(); it++) {
                    std::cerr << it->Type() << std::endl;
                }
            }
            return *this;
		}


        Container& Container::debugDumpContainer(std::ostream &os) {
            os << "HEY !" << std::endl;
            return *this;
        }
    }
}

