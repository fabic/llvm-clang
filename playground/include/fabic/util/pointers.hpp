
#ifndef FABICCPPPLAYGROUND_POINTERS_HPP
#define FABICCPPPLAYGROUND_POINTERS_HPP

#include <memory>
#include <boost/format.hpp>

// #define format_address(x)    (boost::format(" [%x] ") % (x))
// #define format_address_of(x) (format_address(std::addressof(x)))

TL_NS_BEGIN
  namespace ptr {

    /**
     *
     * Why ?
     *  “ Note that prior to calling shared_from_this on an object t,
     *    there must be a std::shared_ptr that owns t. ”
     *    __ http://en.cppreference.com/w/cpp/memory/enable_shared_from_this __
     *
     * @link http://stackoverflow.com/a/16083526
     * @link http://stackoverflow.com/a/15550262
     * @link http://stackoverflow.com/a/12791172
     * @link http://stackoverflow.com/a/32172486
     * @link http://www.boost.org/doc/libs/1_38_0/libs/smart_ptr/sp_techniques.html#from%5Fthis
     *       ( Boost C++ 1.38.0 smart_ptr lib. doc. about what & why was that
     *         which became the std::enable_shared_from_this<> abstraction )
     *
     * https://www.evernote.com/shard/s105/sh/26389c51-7de4-47ba-a4c3-f5426b81f59d/0efedcbdbf3939df36e8fd39a6376654
     */
    template< class Base >
    class inheritable_shared_from_this
      : public std::enable_shared_from_this< Base >
    {
    private:
      /// Unfortunate shared_ptr< self > that is initialized from the below ctor
      /// “automatically” upon (heap only!) instanciation of a concrete class.
      std::shared_ptr< inheritable_shared_from_this > auto_inited_shared_ptr_base_;

    protected:
      /**
       * Here happens the unfortunate magic : always having a shared_ptr< self >
       * that takes ownership of this, so that `std::enable_shared_from_this< Base >`
       * immediately has its weak_ptr<...> thing initialized.
       */
      inheritable_shared_from_this()
        : auto_inited_shared_ptr_base_( this )
      {
        /* No op. ; but it might have been cool if we had a means by which we
           could test that _this_ is a heap-allocated thing; and not something
           from the stack or global static... */
      }

    public:
      virtual ~inheritable_shared_from_this() { }

      template< class Concrete >
      inline
      std::shared_ptr< Concrete > shared_from_base()
      {
        using ESFT = std::enable_shared_from_this< Base >;

        return std::static_pointer_cast< Concrete >(
            this->ESFT::shared_from_this()
          );
      }

      /**
       * Helper static for instantiating concrete classes that ...
       *
       * So that we have at least one alternative to enforcing client-codes
       * from doing having to resort to unclear logic/semantics _just for us
       * (as opposed to them)_ to have this `enable_shared_from_this<...>`
       * automatic `shared_ptr<...>` ability.
       *
       *     std::shared_ptr< Concrete > sptr_ =
       *       (new Concrete( xcb_ ))
       *         ->shared_from_base< Concrete >() ;
       */
      template< class Concrete, class ...Args >
        static inline std::shared_ptr< Concrete >
          _construct(Args&& ...args)
      {
        auto concr_ = new Concrete( std::forward<Args>(args)... );
        auto base_  = static_cast< Base * >( concr_ );
        return base_->template shared_from_base< Concrete >();
      }
    };

  } // ptr ns.

  namespace util { // todo: move to ns 'pointers'

    /**
     * Naked pointer returned as is.
     *
     * @param ptr
     * @return
     */
    template<typename T>
    const T *
    address_of(const T * ptr) {
      return ptr;
    }

    template<typename T>
    const T *
    address_of(const T& ref) {
      return &ref;
    }

    // template<typename Y, typename T = std::shared_ptr<Y>>
    // const Y *
    // addressof(T&& v) {
    //   return v.get();
    // }

//
//    template<typename T>
//    boost::format
//    format_address_of(T& v) {
//      return boost::format(" [%x] ") % address_of(v);
//    }

/* failed.
    template<typename T>
    decltype(T::element_type)
    format_address_of(T&& v) {
      return boost::format("%x") % (addressof(v));
    }
*/

    /** Should work work `std::xxxx_ptr<T>` types, these provide a `get()`
     * method for retrieving the raw/naked pointer.
     *
     * @param v
     * @return ex. `std::shared_ptr< T::element_type >.get()`
     * /
    template<typename T>
    const typename T::element_type *
    address_of(const T& v) {
      return v.get();
    } */

    template<typename T>
    const T *
    address_of(const std::shared_ptr<T>& v) {
      return v.get();
    }

    template<typename T>
    const T *
    address_of(const std::unique_ptr<T>& v) {
      return v.get();
    }

    template<typename T>
    const T *
    address_of(const std::weak_ptr<T>& v) {
      return v.get();
    }

} // util ns
TL_NS_END

#endif //FABICCPPPLAYGROUND_POINTERS_HPP
