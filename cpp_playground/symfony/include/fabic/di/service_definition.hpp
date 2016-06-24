//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP
#define FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP

#include "fabic/di/type_info.hpp"

namespace fabic {
  namespace di{

    using std::string;
    using std::map;
    using std::pair;

    // Forward decl.
    class base_service;


    /**
     * Base abstract class for representing service dependencies.
     */
    class base_dependency_declaration {
    private:
        string service_id;
    public:
        base_dependency_declaration(string service_id) : service_id(service_id) {}
        virtual ~base_dependency_declaration() {}

        string get_service_id() { return this->service_id; }

        virtual const type_info& get_service_type() const {
            throw new std::exception();
        }

        /**
         * Tell whether this dependency is now/presently bound to an actual service
         * definition, i.e. dependency resolution was performed and succeeded for
         * the given service.
         */
        virtual bool has_service() const =0;

        // NOTE: No virtual `set_service(...)` definition: we'll rely on static
        //       (compile-time) polymorphism for this one.
        virtual void set_service(std::shared_ptr<base_service> serv) =0;
    };




    /**
     * Basic implementation of a dependency for a given service “ identifier ”
     * which is (_must be_) of type “ T ”.
     */
    template<class T, class PointerT = std::shared_ptr<T>>
    class dependency_declaration : public base_dependency_declaration {
    public:
        typedef service<T, PointerT>       service_t;
        typedef std::shared_ptr<service_t> service_ptr_t;
    private:
        type_info type;
        /// Will end up storing a pointer to the actual service upon dependency resolution.
        service_ptr_t service_;
    public:
        dependency_declaration(string service_id)
            : base_dependency_declaration(service_id),
              type(typeid(T), false),
              service_(nullptr) {}

        virtual const type_info& get_service_type() const {
            return this->type;
        }

        virtual bool has_service() const {
            return this->service_.get() != nullptr;
        }

        virtual void set_service(std::shared_ptr<base_service> serv) {
            auto ptr = std::dynamic_pointer_cast<service_t>( serv );
            if (ptr == nullptr)
                throw new std::exception();
            this->service_ = ptr;
        }

        /**
         * Set the “ service definition ” that was for this dependency.
         */
        // template<class S, class PointerS>
        // void set_service( service_ptr_t serv ) {
        //     this->service_ = serv;
        // }
    };


    /**
     * Base abstract class for a “ service definition ”.
     */
    class base_service {
    private:
        // Prevent client codes from having implicit copies.
        base_service(const base_service&) = delete;
        base_service& operator=(const base_service&) = delete;
    public:
        typedef std::shared_ptr<base_service> pointer;
        typedef map<string, base_dependency_declaration *> dependencies_map;
        typedef typename boost::call_traits<dependencies_map>::reference dependencies_map_ref;
        typedef typename boost::call_traits<dependencies_map>::const_reference dependencies_map_cref;
    protected:
        string id_;
        // Depth-first post-order search
        bool dfs_resolving_ = false;
        bool dfs_visited_   = false;
    public:
        explicit base_service(string name) : id_(name) {}

        virtual ~base_service() {}

        /**
         * @return the service identifier / name.
         */
        string id() { return this->id_; }

        virtual type_info& get_type_info() throw(std::exception) {
            throw new std::exception();
        }

        // FIXME: temp.
        string get_service_definition_type_name() {
            return type_info::demangle_cxx_type_name(typeid(*this).name());
        }

        virtual dependencies_map_cref get_dependencies_map() const {
            throw new std::exception();
        }

        virtual bool has_instance() const =0;

        virtual void construct() =0;

        bool is_resolving_in_progress() const { return this->dfs_resolving_ ; }
        bool was_visited() const { return this->dfs_visited_ ; }

        void set_resolving_in_progress(bool b) { this->dfs_resolving_ = b ;}
        void set_visited(bool b) { this->dfs_visited_ = b ;}
    };

    /**
     * A service definition
     */
    template<class T, class PointerT = std::shared_ptr<T>>
    class service : public base_service {
    public:
        typedef typename boost::call_traits<service<T,PointerT>>::reference reference;
        typedef typename std::shared_ptr<service<T,PointerT>> pointer;

        typedef PointerT factory_function_prototype(dependencies_map_ref deps);
        typedef std::function<factory_function_prototype> factory_function_t;

        typedef bool starter_function_prototype(pointer service);
        typedef std::function<starter_function_prototype> starter_function_t;

        class no_defined_factory_functor : std::exception {};

    private:
        type_info type;
        dependencies_map dependencies;
        factory_function_t factory;
        starter_function_t starter_function;
        PointerT instance;

    public:
        service(string service_id)
            : base_service(service_id),
              type(typeid(T), false),
              factory(),
              instance()
        { }

        service(string id, PointerT ptr) : service(id) {}

        virtual ~service() {}

        virtual type_info& get_type_info() noexcept { return this->type; }

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
            logtrace("Service "
                         << this->id()
                         << " is-a " << this->get_service_definition_type_name()
                         << ", requires(" << service_id << ")"
                         << ", address : " << format_address_of(*this));

            auto pair = this->dependencies.insert(
                std::make_pair(
                    service_id,
                    new dependency_declaration<Over>(service_id)
                ));

            bool success = pair.second;
            if (!success)
                throw new std::exception();

            auto it = pair.first;

            logtrace(" » inserted dependency '" << it->second->get_service_id() << "' is-a "
                                                << type_info(*it->second).name());

            logtrace(" » dependencies_map contains " << this->dependencies.size() << " elements.");
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
            this->instance = this->factory( this->dependencies );
        }

        virtual bool has_instance() const {
            return this->instance != nullptr;
        }

        PointerT get_instance() {
            if (! this->has_instance())
                throw new std::exception();
            return this->instance;
        }
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
