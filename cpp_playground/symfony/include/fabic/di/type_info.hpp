//
// Created by cadet on 6/24/16.
//

#ifndef FABICCPPPLAYGROUND_TYPE_INFO_HPP
#define FABICCPPPLAYGROUND_TYPE_INFO_HPP

namespace fabic {
  namespace di {

    using std::string;

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

  } // di ns.
} // fabic ns.
#endif //FABICCPPPLAYGROUND_TYPE_INFO_HPP
