//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_TYPE_INFO_HPP
#define FABICCPPPLAYGROUND_TYPE_INFO_HPP

//#include <typeinfo>
// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_typeindex/getting_started.html
#include <boost/type_index.hpp>
#include <cxxabi.h>

// todo: ?
# include "dude/object.hpp"

namespace di {

  using std::string;
  using boost::typeindex::type_index;

  /**
   *
   * Type information in C++, huh! -_-
   *
   * That which everyone will tell you one shall never do, not even attempt to do
   * _(but those have secretly tried it when they where younger)_.
   *
   * EDIT: Cool, found Boost.type_index impl.
   *
   * \link http://www.boost.org/doc/libs/1_61_0/doc/html/boost_typeindex/getting_started.html
   *
   */
  class FABIC_SYMBOL_EXPORT type_info
      // : public stl_type_index
      // ^ Don't specialize this one: it keeps a pointer to the std::type_info instance,
      //   which happens to be on the stack from time to time -_- wtf /me hoped "static
      //   storage duration" meant anywhere but on the stack -_-
  {
  private:
    string name_;
  public:
      type_info(const type_index& tind)
          : name_(tind.pretty_name())
      { }

    /**
     * Helper (short-cut) for static call to
     * `boost::typeindex::stl_type_index::type_id()`.
     *
     * @return
     */
    template <class T>
    inline static
    type_info
    type_id() noexcept
    {
      return type_info(
          type_index::type_id<T>()
      );
    }

    template <class T>
    inline static
    type_info
    type_id_runtime(const T& value) noexcept
    {
      return type_info(
          type_index::type_id_runtime(value)
      );
    }

    inline const string& name() const { return this->name_; }

    // BC
    inline const string& pretty_name() const { return this->name(); }

      /**
       * Demangles the C++ type name through `abi::__cxa_demangle()`.
       *
       * todo: see `boost::core::demangle` from `boost/dll/detail/demangling/demangle_symbol.hpp`
       * todo: remove this, or move it to hack NS, and/or have it be a friend func.
       */
      BOOST_SYMBOL_EXPORT
      static string demangle_cxx_type_name(const char *mangled_name);
  };

} // di ns.

#endif //FABICCPPPLAYGROUND_TYPE_INFO_HPP
