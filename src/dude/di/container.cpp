//
// Created by fabi on 11/21/15.
//

//#include <yaml-cpp/yaml.h>
#include <iostream>

# include "dude/di/container.hpp"
# include "dude/di/service_provider.hpp"

namespace di {

  // Ctor
  // private btw.
  service_container::service_container() {}


  // static btw.
  container_shared_ptr_t
    service_container::new_container_instance()
  {
    // The container instance, managed by shared_ptr.
    auto container = container_shared_ptr_t(
        new service_container()
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


  service_container::reference
    service_container::register_service(
        base_definition_shared_ptr_t service
      )
  {
    this->services_.insert(service);
    return *this;
  };


  service_container::service_ptr_t
    service_container::service_map::find(string id)
  {
    auto it = this->services_.find(id);

    if (it == this->services_.end())
      BOOST_THROW_EXCEPTION( service_not_found_exception() );

    auto service = it->second;

    return service;
  }


  service_container::service_map::reference
    service_container::service_map::insert(
        base_definition::pointer service
      )
  {
    auto pair = this->services_.insert(
        make_pair(
          service->id(),
          service
        )
      );

    bool success = pair.second;
    if (!success) {
      //auto it = pair.first;
      // TODO: ^ forward sthg along w/ the exception.
      BOOST_THROW_EXCEPTION( service_already_exists_exception() );
    }

    return *this;
  };


//       service_container& service_container::loadFromYamlFile(string filename) {
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


  service_container&
    service_container::debugDumpContainer()
  {
    logdebug << "CONTAINER DEBUG DUMP (1 - basic) :";

    for (const auto &pair : this->services_.get_map_impl()) {
      service_ptr_t base = pair.second;
      logdebug << " » " << base->id() << " [" << dude::util::address_of(base) << "]";
    }

    logdebug << "CONTAINER DEBUG DUMP (2 - detail) :";

    for (const auto &pair : this->services_.get_map_impl()) {
      service_ptr_t base = pair.second;

      logdebug << "  » " << base->id() << " : " << std::endl
         << "    - is-a:     " << base->get_service_definition_type_name() << std::endl
         << "    - address : " << " [" << dude::util::address_of(base) << "]" << std::endl
         << "    - type:     " << base->get_type_info().pretty_name() << std::endl;

     auto deps = base->get_dependencies_map();

     if (! deps.empty() ) {
       logdebug << "    - dependencies :";

       for (const auto &tuple : deps) {
         base_dependency_declaration &dep = *tuple.second;
         logdebug << "        - " << dep.get_service_id()
            << " as " << dep.get_service_type().name()
            << " [" << dude::util::address_of(dep) << "]";
       }
      }
      else {
       logdebug << "    - dependencies : NONE.";
      }
    }

    return *this;
  }


  service_container::service_ptr_t
    service_container::resolve_service_dependencies(string id)
  {
    logtrace << "service_container::resolve_service_dependencies('" << id << "')";

    service_ptr_t service = this->services_.find(id);

    logtrace << " » found service: " << service->id()
             << ", got a " << service->get_service_definition_type_name();

    if (service->was_visited())
      return service;
    else if (service->is_resolving_in_progress())
      return nullptr; // todo: throw ex. ? is this a cycle ?

    logtrace << " » resolving dependencies...";

    service->set_resolving_in_progress(true);

    auto dependencies = service->get_dependencies_map();

    logtrace << " » dependencies map contains "
             << dependencies.size() << " elements.";

    for (auto pair : dependencies) {
      auto depdecl = pair.second;

      logtrace << "     - depends on service "
               << depdecl->get_service_id()
               << ", type: " << depdecl->get_service_type().pretty_name();

      service_ptr_t serv = this->resolve_service_dependencies(
          depdecl->get_service_id()
      );

      logtrace << " » " << service->id()
               << " : finished processing dep. : " << depdecl->get_service_id();

      depdecl->set_service(serv);
    }

    logtrace << " » " << service->id() << " : end of dependencies resolution.";
    logtrace << " » " << service->id() << " : about to construct service.";

    service->construct();

    logtrace << " » " << service->id() << " : construct() done.";

    service->set_resolving_in_progress(false);
    service->set_visited(true);

    return service;
  }


  void
  service_container::start_startable_services()
  {
    logtrace << "service_container::start_startable_services(): begin.";

    // todo: have a custom container::for_each(...) and for_each_matching()... things...

    for (const auto& pair : this->services_.get_map_impl())
    {
      service_ptr_t base = pair.second;

      if (! base->is_startable()) {
        logtrace << " » Skipping non-startable service " << base->id();
        continue;
      }

      service_ptr_t tmp = this->resolve_service_dependencies( base->id() );

      // resolution should really have resolved the _same_ service definition.
      assert( tmp == base );

      logtrace << " » starting service " << base->id();

      base->start();

      logtrace << " » started service " << base->id();
    }

    logtrace << "service_container::start_startable_services(): end.";
  }


  // static btw.
  // fixme: note that this is an exported symbol, see definition [quickfix].
  // todo: see Boost.DLL: it appears to have a demangling func...
  // todo: move this elsewhere
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

