//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_DEPENDENCY_HPP
#define FABICCPPPLAYGROUND_DEPENDENCY_HPP

# include "dude/di/typedefs.hpp"
# include "dude/di/type_info.hpp"

namespace di {

  using std::string;

  // Forward decl.
  class base_definition;


  /**
   * Base abstract class for representing service dependencies.
   */
  class FABIC_SYMBOL_EXPORT base_dependency_declaration
      : public std::enable_shared_from_this<base_dependency_declaration> {
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
      virtual void set_service(std::shared_ptr<base_definition> serv) =0;
  };


  /**
   * Basic implementation of a dependency for a given service “ identifier ”
   * which is (_must be_) of type “ T ”.
   */
  template<class T, class PointerT = std::shared_ptr<T>>
  class FABIC_SYMBOL_EXPORT dependency_declaration : public base_dependency_declaration {
  public:
      typedef definition<T, PointerT>       service_t;
      typedef std::shared_ptr<service_t> service_ptr_t;
  private:
      type_info type;
      /// Will end up storing a pointer to the actual service upon dependency resolution.
      service_ptr_t service_;
  public:
      dependency_declaration(string service_id)
          : base_dependency_declaration(service_id),
            type(type_info::type_id<T>()),
            service_(nullptr) {}

      virtual const type_info& get_service_type() const {
          return this->type;
      }

      virtual bool has_service() const {
          return this->service_.get() != nullptr;
      }

      virtual void set_service(std::shared_ptr<base_definition> serv) {
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

} // di ns.

#endif //FABICCPPPLAYGROUND_DEPENDENCY_HPP
