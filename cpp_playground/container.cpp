//
// Created by fabi on 11/21/15.
//

#include <yaml-cpp/yaml.h>
#include <iostream>
#include "container.hpp"

namespace fabic {
namespace di {


Container::Container() {
    auto self = std::make_shared< service<Container> >(
        "container",
        std::shared_ptr<Container>(this));
    this->register_service( self );
}


Container::service_ptr_t
Container::service_map::find(string id) throw(Container::service_not_found_exception)
{
    auto it = this->services_.find( id );

    if (it == this->services_.end())
        throw new service_not_found_exception();

    auto service = it->second;

    return service;
}

//       Container& Container::loadFromYamlFile(string filename) {
//        YAML::Node yaml = YAML::LoadFile(filename);
//           std::cerr << yaml.Type() << std::endl;
//           const auto& srvs = yaml["services"];
//           if (srvs.IsDefined()) {
//               for(auto it = srvs.begin(); it != srvs.end(); it++) {
//                   std::cerr << it->Type() << std::endl;
//               }
//           }
//           return *this;
//       }


Container& Container::debugDumpContainer(std::ostream &os)
{
    os << "HEY !" << std::endl;

    for(const auto& pair : this->services) {
        base_service * base = pair.second;
        os << base->id() << " : "
           << base->get_service_definition_type_name()
           << ", address: " << format_address_of(base);

        os << ", type: " << base->get_type_info().name();

        os << std::endl;

        auto deps = base->get_dependencies_map();
        for(const auto& tuple : deps) {
            base_dependency_declaration& dep = *tuple.second;
            os << "  » " << dep.get_service_id()
               << " as " << dep.get_service_type().name()
               << ", address: " << format_address_of(&dep)
               << std::endl;
        }
    }

    return *this;
}


void Container::resolve_service_dependencies(string id) {
    logtrace("Container::resolve_service_dependencies('" << id << "')");

    service_ptr_t service = this->services_.find(id);

    logtrace(" » found service: " << service->id() << ", got a " << service->get_service_definition_type_name());

    auto dependencies = service->get_dependencies_map();

    logtrace(" » dependencies map contains " << dependencies.size() << " elements.");

    for(auto pair : dependencies) {
        auto depdecl = pair.second;
        std::cout << depdecl->get_service_type().name();
    }
}


// static btw.
string
type_info::demangle_cxx_type_name(const char *mangled_name)
{
    int status = 0xdeadbeef;

    // See cxxabi.h for the documentation.
    // PS: we're responsible for free()-ing the char* buffer.
    char *name = abi::__cxa_demangle(
            mangled_name,
            nullptr, // buffer
            nullptr, // buffer length
            &status  // demangling exit status
    );

    // Return as-is in case of error.
    if (name == nullptr || status != 0)
        return string(mangled_name);

    auto name_s = string(name);

    free(name);

    return name_s;
}

} // di ns.
} // fabic ns.

