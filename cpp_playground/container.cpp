//
// Created by fabi on 11/21/15.
//

#include "container.hpp"

namespace fabic {
    namespace di {

        Container::Container() {}

        void * Container::get_service(string id) {
        	auto it = services.find(id);
        	if (it == services.end())
        		return nullptr;
        	return it->second.second;
        }

    }
}

