#ifndef SF_COLOR_HPP
#define SF_COLOR_HPP

#include <c++/v1/cstdint>

namespace sf {


  template< typename Scalar = uint8_t >
  struct rgba {
    Scalar r = 0;
    Scalar g = 0;
    Scalar b = 0;
    Scalar a = 0;

    inline rgba(Scalar r, Scalar g, Scalar b, Scalar a)
        : r(r), g(g), b(b), a(a)
    { }

    /**
     * Converting constructor : if _this_ Scalar is floating-point,
     * and other (T) type is integral, then we're scaling to unit values.
     *
     * \param other
     */
    template< typename T >
    rgba< Scalar >(const rgba< T >& other)
    {
      static_assert( std::is_arithmetic<T>::value );

      if (std::is_floating_point< Scalar >::value
          && std::is_integral< T >::value)
      {
        constexpr T max = std::numeric_limits<T >::max();
        this->r = (Scalar) other.r / (Scalar) max;
        this->g = (Scalar) other.g / (Scalar) max;
        this->b = (Scalar) other.b / (Scalar) max;
        this->a = (Scalar) other.a / (Scalar) max;
      }
      else {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        this->a = other.a;
      }
    }
  };


} // sf ns.
#endif // SF_COLOR_HPP
