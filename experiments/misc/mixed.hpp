#ifndef FABIC_MIXED_DATATYPE
#define FABIC_MIXED_DATATYPE

#include <map>
#include <memory>
#include <forward_list>
#include <string>
#include <iostream>
//#include <typeinfo>

    namespace types {

        using std::string;
        using std::map;
        using std::pair;

        class mixed {
        private:
            struct {
                int is_null:1;
                int is_boolean:1;
                int is_integer:1;
                int is_real:1;
                int is_string:1;
            } bits;

            union {
                union {
                    bool   b; // boolean type.
                    long   i; // signed integer type
                    double d; // floating point type.
                } w;
                char s[sizeof(w)];
            } u;

            constexpr static auto sz = sizeof( decltype(u) );


        public:
            mixed() {
                std::cout << "HUH: typeid : "    << typeid(decltype(u)).name() << std::endl;
                std::cout << "HUH: sizeof(u)   : " << sizeof(u)   << std::endl;
                std::cout << "HUH: sizeof(u.w) : " << sizeof(u.w) << std::endl;
                std::cout << "HUH: sizeof(u.s) : " << sizeof(u.s) << std::endl;
                std::cout << "HUH: sizeof(bits) : " << sizeof(bits) << std::endl;
            }
        };

    } // types ns.

#endif // FABIC_MIXED_DATATYPE
