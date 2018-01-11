#ifndef CLONG_PLUGIN_PQXXHelper_HPP
#define CLONG_PLUGIN_PQXXHelper_HPP

#include <cassert>
#include <pqxx/pqxx>
#include "exceptions.hpp"

namespace clong {


  /**
   * "Hack" helper for having the ability to delay the initialization of
   * some class `Class`.  _This **is** a hack_ that resort to C++ placement new.
   *
   * Useful in cases where some third-party class require that its instance be
   * initialized (no default constructor) and prevents copying, and you really
   * want to embed that instance in your class declaration.
   *
   * TODO: review this doc., explain clearly.
   * TODO: find a better name for this, HackedInstance?
   * TODO: refactor this elsewhere, ex. dude/hacks/
   */
  template<typename Class>
    class DelayedInstance {
    public:
      using Self = DelayedInstance< Class > ;

    private:
      char MemoryArea_[ sizeof(Class) ];
      bool IsInitialized_ = false;

    public:
      /// ctor.
      explicit DelayedInstance() { /* noop */ }

      /// dtor.
      ~DelayedInstance() {
        // TODO: Deconstruct().
        if (IsInitialized_)
          Destruct();
      }

      /// Return true if `Class` was instanciated.
      bool Exists() const { return IsInitialized_ == true ; }

      /// Construct an instance of `Class` into the memory area `MemoryArea_`
      /// by doing a placement new.
      template<typename ...CtorArgs>
        Class* Construct(CtorArgs&&... Args)
        {
          if (IsInitialized_)
            throw std::runtime_error("Dough! Ist initialized already!");
          Class* Ptr = new (MemoryArea_) Class (Args...);
          // paranoid: couldn't be otherwise.
          assert( reinterpret_cast<char*>(Ptr) == MemoryArea_ );
          // paranoid (and useless): we want this to coincide with the memory
          //                         buffer base address.
          assert( reinterpret_cast<void*>(Ptr) == reinterpret_cast<void*>(this));
          IsInitialized_ = true;
          return Ptr ;
        }

      /// Invoke the destructor of `Class`, and set state as un-initialized.
      Self& Destruct()
      {
        operator->()->~Class();
        IsInitialized_ = false;
        return *this ;
      }

      /// Smart-pointer like behaviour: return a reference.
      Class& operator*() {
          if ( ! IsInitialized_ )
            throw std::runtime_error("Dough!");
          return *operator->() ;
      }

      /// Smart-pointer like behaviour: return a pointer.
      Class* operator->() {
          if ( ! IsInitialized_ )
            throw std::runtime_error("Dough!");
          return reinterpret_cast< Class* >( MemoryArea_ ) ;
      }
    };


  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


  /**
   * PostreSQL database connectivity helper.
   *
   * Using a lazy connection since we're typically being setup from
   * constructors (e.g. Clong class) and pqxx happens to throw exceptions in
   * case of database connectivity errors, which ends up crashing Clang badly.
   *
   * TODO: We deal a lot with StringRef in Clang, and we currently resort to
   * TODO: StringRef::str() so as to obtain a std::string, which sucks a bit
   * TODO: (extraneous memory allocations) ~~> maybe we can impl. a custom
   * TODO: method that tap into `libpq` directly ?
   *
   * TODO: impl. savepoints ? have a SmallVector<...> stack ?
   */
  class PQXXHelper : public pqxx::lazyconnection {
    using Base = pqxx::lazyconnection ;
  private:
    /// Will hold a transaction instance when it is needed, see `TXN()`.
    DelayedInstance< pqxx::transaction<> > TXN_ ;
  public:
    explicit PQXXHelper(const char *OptionsStr);

    /// Return a reference to the transaction, creating one if needed.
    /// Note that there may only be _one_ transaction at all time;
    /// FYI: PostgreSql does not support nested transaction, which is fine;
    /// FYI: but it does have a SAVEPOINT facility, that is used by `pqxx`
    /// in their `subtransaction` impl.
    pqxx::transaction<>&
      TXN() {
        if (! TXN_.Exists())
          TXN_.Construct( *this );
        return *TXN_;
      }

    /**
     * Helper around libpqxx's `exec_params1()` for running an INSERT statement
     * that has a `RETURNING id` clause.
     *
     * FIXME: This is misleadingly called Insert() but it really doesn't care
     * FIXME: about the provided SQL ~~> rename ?
     * TODO: log SQLs.
     */
    template<typename IdentifierType = unsigned int,
             typename ...Arguments>
      IdentifierType
        Insert(const std::string& SQL, Arguments&&... Args)
        {
          pqxx::row R = TXN().exec_params1(SQL, Args...);
          IdentifierType ID = R[0].as< IdentifierType >();
          return ID;
        }

    /// Helper for committing the transaction.
    /// TODO: Eventually we may implement "sub-transactions" (savepoints)
    /// TODO: and this method may basically acknowledge the most recent
    /// TODO: savepoint (release...).
    PQXXHelper& Commit() {
      if (! TXN_.Exists())
        throw clong_error("There's no transaction at the moment!"
                          " Can't possibly commit a thing -_-");
      TXN().commit();
      return *this;
    }
  };

} // clong ns


#endif // CLONG_PLUGIN_PQXXHelper_HPP
