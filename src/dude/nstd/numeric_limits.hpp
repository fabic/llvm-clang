#ifndef _NSTD_NUMERIC_LIMITS_H
#define _NSTD_NUMERIC_LIMITS_H

/*
 * \since 2017-02-17 stole these from Clang's libcxx
 *
 * * libcxx/include/type_traits
 * * libcxx/include/limits
 */

namespace nstd {

  // remove_const/volatile/cv ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T> struct remove_const          { typedef T type; };
  template <class T> struct remove_const<const T> { typedef T type; };

  template <class T> struct remove_volatile             { typedef T type; };
  template <class T> struct remove_volatile<volatile T> { typedef T type; };

  /// Remove const-volatile.
  template <class T> struct remove_cv
  {
    typedef typename remove_volatile<
        typename remove_const<T>::type
      >::type type;
  };

  // }

  // integral / bool constant, true/false type ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T, T _val>
    struct integral_constant
    {
        static constexpr const T      value = _val;
        typedef T                     value_type;
        typedef integral_constant     type;

        inline constexpr operator value_type() const noexcept {return value;}
        inline constexpr value_type operator ()() const noexcept {return value;}
    };

  // fixme: can't tell what's the purpose of that construct.
  // template <class T, T _val>
  //   constexpr const T integral_constant<T, _val>::value;

  template <bool _val>
    using bool_constant = integral_constant<bool, _val>;

  using  true_type = bool_constant< true >;
  using false_type = bool_constant< false >;

  // }


  // ~ ~ is_integral ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  namespace priv {
    template <class T>   struct is_integral                     : public false_type {};
    template <>          struct is_integral<bool>               : public true_type {};
    template <>          struct is_integral<char>               : public true_type {};
    template <>          struct is_integral<signed char>        : public true_type {};
    template <>          struct is_integral<unsigned char>      : public true_type {};
    template <>          struct is_integral<short>              : public true_type {};
    template <>          struct is_integral<unsigned short>     : public true_type {};
    template <>          struct is_integral<int>                : public true_type {};
    template <>          struct is_integral<unsigned int>       : public true_type {};
    template <>          struct is_integral<long>               : public true_type {};
    template <>          struct is_integral<unsigned long>      : public true_type {};
    template <>          struct is_integral<long long>          : public true_type {};
    template <>          struct is_integral<unsigned long long> : public true_type {};
  }

  template <class T>
    struct is_integral
      : public priv::is_integral< typename remove_cv<T>::type > {};

  // }


  // ~ ~ is_floating_point ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  namespace priv {
    template <class T>   struct is_floating_point              : public false_type {};
    template <>          struct is_floating_point<float>       : public true_type {};
    template <>          struct is_floating_point<double>      : public true_type {};
    template <>          struct is_floating_point<long double> : public true_type {};
  }

  template <class T>
    struct is_floating_point
      : public priv::is_floating_point< typename remove_cv<T>::type > {};

  // }


  // ~ ~ is_arithmetic ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  template <class T>
    struct is_arithmetic
      : public integral_constant<bool, is_integral<T>::value ||
                                       is_floating_point<T>::value> {};

  // }


  // ~ ~ numeric_limits<T> ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ {

  namespace priv {

    template <class T, bool = is_arithmetic<T>()>
      class numeric_limits {};

    template <class T>
      class numeric_limits<T, true>
      {
      public:
          typedef T type;

          static constexpr bool is_signed = type(-1) < type(0);

          /// Count of boolean digits.
          static constexpr int digits = static_cast<int>(sizeof(type) * 8 - is_signed);

          inline static constexpr type min() noexcept {
            return type( 1UL << digits );
            //return (type) (1UL << digits);
            // FIXME: Clang complains “warning: shift count >= width of type [-Wshift-count-overflow]”
          }

          inline static constexpr type max() noexcept {
            return is_signed ? type(type(~0) ^ min()): type(~0);
          }
      };

  } // priv ns

  /**
   * \link http://en.cppreference.com/w/cpp/types/numeric_limits
   */
  template<class T>
    class numeric_limits
      : public priv::numeric_limits< typename remove_cv<T>::type > {};

  static_assert(numeric_limits<char>::max()          ==  127);
  static_assert(numeric_limits<char>::min()          == -128);

  static_assert(numeric_limits<unsigned char>::min() ==   0);
  static_assert(numeric_limits<unsigned char>::max() == 255);

  static_assert(numeric_limits<unsigned int>::min() ==          0);
  static_assert(numeric_limits<unsigned int>::max() == 4294967295);

  static_assert(numeric_limits<int>::max() ==  2147483647);
  static_assert(numeric_limits<int>::min() == -2147483648);

  static_assert(numeric_limits<unsigned long>::max() == 18446744073709551615UL);
  static_assert(numeric_limits<long>::max() ==  9223372036854775807UL);
  static_assert(numeric_limits<long>::min() == -9223372036854775808UL);

  // }

} // nstd ns.

#endif // _NSTD_NUMERIC_LIMITS_H
