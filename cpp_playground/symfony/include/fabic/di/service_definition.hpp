//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP
#define FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP


    /**
     * A service definition
     */
    template<class T, class PointerT = std::shared_ptr<T>>
    class service : public base_definition {
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
            : base_definition(service_id),
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
            logtrace << "Service "
                      << this->id()
                      << " is-a "       << this->get_service_definition_type_name()
                      << ", requires("  << service_id << ")"
                      << ", address : " << format_address_of(*this) ;

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
                      << "' is-a " << type_info(*it->second).name() ;

            logtrace << " » dependencies_map contains "
                      << this->dependencies.size()
                      << " elements." ;

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

#endif //FABICCPPPLAYGROUND_SERVICE_DEFINITION_HPP
