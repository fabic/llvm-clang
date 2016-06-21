#ifndef FABIC_DI_CONTAINER
#define FABIC_DI_CONTAINER

#include <map>
#include <memory>
#include <forward_list>
#include <string>
#include <ostream>
#include <cstdio>
#include <typeinfo>
#include <type_traits>
#include <functional>
#include <memory>

#include <cxxabi.h>
#include <boost/format.hpp>
#include <boost/call_traits.hpp>

//#include <boost/log/trivial.hpp>
// ^ fixme: linking pb.

#ifdef BOOST_LOG_TRIVIAL
#  define logtrace() BOOST_LOG_TRIVIAL(trace)
#else
#  define logtrace(message) std::cerr << "TRACE: " << message << std::endl
#endif

#define format_address_of(x) (boost::format("%x") % x)

#include "object.hpp"

namespace fabic {
namespace di {

using std::string;
using std::map;
using std::pair;


/**
 *
 * Type information in C++, huh! -_-
 *
 * That which everyone will tell you one shall never do, not even attempt to do
 * _(but those have secretly tried it when they where younger)_.
 *
 */
class type_info {
private:
    string mangled_name_;
    string type_name_;
    bool is_object_instance_;
public:
    type_info(const std::type_info& type, bool _unused)
            : mangled_name_(type.name()),
              type_name_(demangle_cxx_type_name(mangled_name_.c_str())),
              is_object_instance_(false) // fixme
    { }

    /**
     * Templated ctor.
     *
     * @param v
     * @return
     */
    template<typename T>
    type_info(T& v)
            : mangled_name_(typeid(v).name()),
              type_name_(demangle_cxx_type_name(mangled_name_.c_str())),
              is_object_instance_(std::is_convertible<T, object>::value)
    { }

    string name() const { return this->type_name_; }

    bool is_object_instance() const { return this->is_object_instance_; }

    /**
     * Demangles the C++ type name through `abi::__cxa_demangle()`.
     */
    static string demangle_cxx_type_name(const char *mangled_name);
};


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

};


/**
 * Basic implementation of a dependency for a given service “ identifier ”
 * which is (_must be_) of type “ T ”.
 */
template<class T>
class dependency_declaration : public base_dependency_declaration {
private:
    type_info type;
public:
    dependency_declaration(string service_id)
            : base_dependency_declaration(service_id),
              type(typeid(T), false) {}

    virtual const type_info& get_service_type() const {
        return this->type;
    }
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
    typedef map<string, base_dependency_declaration *> dependencies_map;
    typedef typename boost::call_traits<dependencies_map>::reference dependencies_map_ref;
    typedef typename boost::call_traits<dependencies_map>::const_reference dependencies_map_cref;
protected:
    string id_;
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

    virtual void construct() throw(std::exception) {
        throw new std::exception();
    }
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
private:
    type_info type;
    dependencies_map dependencies;
    factory_function_t factory;
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
            << ", address : " << format_address_of(this));

        auto pair = this->dependencies.insert(
            std::make_pair(
                service_id,
                new dependency_declaration<Over>(service_id)));

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

    service<T>&
    set_factory_function(factory_function_t functor) {
        this->factory = functor;
        return *this;
    }

    virtual void construct() throw(std::exception) {
        if (! this->has_instance())
            this->instance = this->factory( this->dependencies );
    }

    bool has_instance() const {
        return this->instance != nullptr;
    }

    PointerT get_instance() {
        this->construct();
        return this->instance;
    }
};

/**
 * Huh! a service container! in C++ ?
 * _yet another one goin' down this road ! why !?_
 *
 *
 * THE DEPENDENCY INJECTION CONTAINER
 *
 *   a.k.a IoC container
 *   a.k.a service container
 *
 */
class Container {
public:
    typedef typename boost::call_traits<Container>::reference reference;
    typedef std::shared_ptr<base_service> service_ptr_t;

    class service_not_found_exception : std::exception {};
    class service_already_exists_exception : std::exception {};

    /**
     * Wrapper around the std::map<>.
     */
    class service_map {
    public:
        typedef map<string, Container::service_ptr_t> map_t;
        typedef typename boost::call_traits<map_t>::reference map_ref;
        typedef typename boost::call_traits<service_map>::reference reference;
    private:
        map_t services_;
    private:
        // Prevent implicit copies :
        service_map(const service_map&) = delete;
        service_map& operator=(const service_map&) = delete;
    public:
        explicit service_map() {}
        map_ref get_map_impl() { return this->services_; }

        /**
         * Add a service to the map.
         *
         * @param service
         * @return self
         */
        template<typename T, class PointerT = std::shared_ptr<T>>
          reference insert(std::shared_ptr<service<T,PointerT>> service)
            throw(service_already_exists_exception)
        {
            auto pair = this->services_.insert( std::make_pair(service->id(), service) );

            bool success = pair.second == true;
            if (!success)
                throw new service_already_exists_exception();

            //auto it = pair.first;

            return *this;
        };

        service_ptr_t find(string id) throw(service_not_found_exception);
    };

private:
    map<string, base_service * > services;
    service_map services_;

private:
    // Prevent implicit copies :
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

public:
    /**
     * Constructor :
     *   - “ this ” container registers itself as service `container`.
     */
    explicit Container();

    /**
     * Register (add) a service definition to this container.
     *
     * @param service
     * @return
     */
    template<typename T, class PointerT = std::shared_ptr<T>>
    reference register_service(std::shared_ptr<service<T,PointerT>> service) {
        this->services_.insert(service);
        return *this;
    };

    /**
     * Fetches a service by its identifier.
     *
     * @param id
     * @return a pointer to the actual thing (typically a shared_ptr<T>).
     */
    template<typename T, class PointerT = std::shared_ptr<T>>
    PointerT get_service(string id) {
        logtrace("get_service('" << id << "')");

        this->resolve_service_dependencies(id);

        service_ptr_t serv = this->services_.find(id);

        logtrace(" » found service: "
            << serv->id()
            << ", got a " << serv->get_service_definition_type_name()
            << ", address: " << format_address_of(&serv));

        auto concrete =
                dynamic_cast< service<T, PointerT> * >(serv.get());

        if (concrete != nullptr) {
            logtrace(" » service is-a : " << concrete->get_type_info().name());
            return concrete->get_instance();
        }
        else {
            throw new std::exception();
        }
    }

    void resolve_service_dependencies(string service_id);

    template<typename T>
    service<T>&
    new_service_definition(string service_id) {
        auto def = new service<T>(service_id);
        auto pair = this->services.insert(std::make_pair(service_id, def));

        bool success = pair.second == true;
        if (!success)
            throw new std::exception();

        auto it = pair.first;

        logtrace("Container::new_service_definition('" << it->second->id() << "'), address: " << format_address_of(it->second));

        return *def;
    }

    Container& loadFromYamlFile(string filename);

    Container& debugDumpContainer(std::ostream& os);
};

} // di ns.
} // fabic ns.

#endif
