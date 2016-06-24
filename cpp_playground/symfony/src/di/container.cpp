//
// Created by fabi on 11/21/15.
//

//#include <yaml-cpp/yaml.h>
#include <iostream>
#include "fabic/di/container.hpp"

namespace fabic {
namespace di {


// private btw.
Container::Container() { }


// static btw.
container_shared_ptr_t
Container::new_container_instance()
{
    // The container instance, managed by shared_ptr.
    auto container = container_shared_ptr_t(
        new Container()
    );

    // Register...
    container->register_service(
        container_service_definition_ptr_t(
            // a new service with ID `container`
            new container_service_definition_t(
                "container",
                // of the above created container
                container
            )
        )
    );

    return container;
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
    os << std::endl
       << "CONTAINER DEBUG DUMP :"
       << std::endl;

    for(const auto& pair : this->services_.get_map_impl())
    {
        service_ptr_t base = pair.second;

        os << "  » " << base->id() << " : " << std::endl
           << "    - is-a:     " << base->get_service_definition_type_name() << std::endl
           << "    - address : " << format_address_of(base)      << std::endl
           << "    - type:     " << base->get_type_info().name() << std::endl
           ;

        auto deps = base->get_dependencies_map();

        os << "    - dependencies :" << std::endl;

        for(const auto& tuple : deps) {
            base_dependency_declaration& dep = *tuple.second;
            os << "        - " << dep.get_service_id()
               << " as " << dep.get_service_type().name()
               << " [" << format_address_of(dep) << "]"
               << std::endl;
        }

        os << std::endl;
    }

    return *this;
}


Container::service_ptr_t
Container::resolve_service_dependencies(string id) {
    logtrace("Container::resolve_service_dependencies('" << id << "')");

    service_ptr_t service = this->services_.find(id);

    logtrace(" » found service: " << service->id() << ", got a " << service->get_service_definition_type_name());

    if ( service->was_visited() )
        return service;
    else if (service->is_resolving_in_progress())
        return nullptr; // todo: throw ex. ? is this a cycle ?

    logtrace(" » resolving dependencies...");

    service->set_resolving_in_progress(true);

    auto dependencies = service->get_dependencies_map();

    logtrace(" » dependencies map contains " << dependencies.size() << " elements.");

    for(auto pair : dependencies) {
        auto depdecl = pair.second;

        logtrace("     - depends on service "
            << depdecl->get_service_id()
            << ", type: " << depdecl->get_service_type().name());

        service_ptr_t serv = this->resolve_service_dependencies(
            depdecl->get_service_id()
        );

        logtrace(" » " << service->id()
            << " : finished processing dep. : " << depdecl->get_service_id());

        depdecl->set_service( serv );
    }

    logtrace(" » " << service->id() << " : end of dependencies resolution.");
    logtrace(" » " << service->id() << " : about to construct service.");

    service->construct();

    logtrace(" » " << service->id() << " : construct() done.");

    service->set_resolving_in_progress(false);
    service->set_visited(true);

    return service;
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

