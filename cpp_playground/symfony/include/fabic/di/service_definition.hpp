//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP
#define FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP

# include <string>
# include <map>

# include "fabic/di/base_definition.hpp"

namespace fabic {
  namespace di {

    using std::string;
    using std::map;
    using std::pair;


    /**
     * A service definition
     */
    template<class T, class PointerT = std::shared_ptr<T>>
    class FABIC_SYMBOL_EXPORT definition : public base_definition {
    public:
      typedef definition<T, PointerT>                         self_t;
      typedef typename std::shared_ptr<self_t>                pointer;   // todo: self_ptr_t ?
      typedef typename boost::call_traits<self_t>::reference  reference; // todo: self_ref_t ?

      typedef PointerT factory_function_prototype(dependencies_map_ref deps);
      typedef std::function<factory_function_prototype> factory_function_t;

      typedef bool starter_function_prototype(reference service);
      typedef std::function<starter_function_prototype> starter_function_t;

      class no_defined_factory_functor : std::exception {}; // todo: refactor ex. out of here.

    private:
      type_info           type;
      dependencies_map    dependencies;
      factory_function_t  factory;
      starter_function_t  starter_function;
      PointerT            instance;

    public:
      /**
       * Ctor
       *
       * @param service_id
       * @return
       */
      definition(string service_id)
          : base_definition(service_id),
            type(type_info::type_id<T>()),
            instance() // i.e. null (typically an empty shared_ptr<T>) pointer.
      { }

      /**
       * Ctor with an existing instance of the thing.
       *
       * @param id
       * @param ptr
       * @return
       */
      definition(string id, PointerT ptr)
          : base_definition(id),
            type(type_info::type_id<T>()),
            instance(ptr)
      { }

      virtual ~definition() {}

      virtual const type_info& get_type_info() const { return this->type; }

      /**
       * Declare (add) a dependency over a given service that has type “ Over ”.
       *
       *     my_serv->requires<DatabaseConnection>("database.connection");
       *     my_serv->requires<HttpRequest>("http.request");
       */
      template<typename Over>
      reference
      requires(string service_id)
      {
        logtrace << "Service "
                 << this->id()
                 << " is-a " << this->get_service_definition_type_name()
                 << ", requires(" << service_id << ")"
                 << ", address : " << util::address_of(this);

        auto pair = this->dependencies.insert(
            std::make_pair(
                service_id,
                new dependency_declaration<Over>(service_id)
            ));

        bool success = pair.second;
        if (!success)
          throw new std::exception();

        auto it = pair.first;

        logtrace << " » inserted dependency '"
                 << it->second->get_service_id()
                 << "' is-a " << type_info(*it->second).name();

        logtrace << " » dependencies_map contains "
                 << this->dependencies.size()
                 << " elements.";

        return *this;
      }

      virtual dependencies_map_cref get_dependencies_map() const {
        return this->dependencies;
      }

      reference
      set_factory_function(factory_function_t functor) {
        this->factory = functor;
        return *this;
      }

      reference
      set_starter_function(starter_function_t functor) {
        this->starter_function = functor;
        return *this;
      }

      virtual void construct() throw(std::exception) {
        if (this->has_instance())
          throw new std::exception();
        else if (this->factory == nullptr)
          throw new no_defined_factory_functor();
        this->instance = this->factory(this->dependencies);
      }

      virtual bool has_instance() const {
        return this->instance != nullptr;
      }

      PointerT get_instance() {
        if (!this->has_instance())
          throw new std::exception();
        return this->instance;
      }

      /**
       * @return true if this service has a starter functor.
       */
      virtual bool is_startable() const {
        return this->starter_function != nullptr;
      }

      virtual bool start() {
        if (! this->is_startable() )
          throw new std::exception(); // todo: throw specific ex. here.
        return this->starter_function( *this );
      };

    };


    /**
     * Interface for life-cycle aware services.
     */
    class startable_service {
    public:
      virtual bool start() =0;
      virtual bool stop() =0;
    };


    /**
     * Base abstract class for services that manage a thread
     * (multiple threads too ?)
     */
    class threaded_service : startable_service {

    };

  } // di ns.
} // fabic ns.
#endif //FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP
