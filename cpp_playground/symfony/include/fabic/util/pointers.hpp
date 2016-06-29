
#ifndef FABICCPPPLAYGROUND_POINTERS_HPP
#define FABICCPPPLAYGROUND_POINTERS_HPP

#include <memory>
#include <boost/format.hpp>

// #define format_address(x)    (boost::format(" [%x] ") % (x))
// #define format_address_of(x) (format_address(std::addressof(x)))

namespace fabic {
  namespace util {

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

  }
}

#endif //FABICCPPPLAYGROUND_POINTERS_HPP
