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

    inline rgba(Scalar r, Scalar g, Scalar b, Scalar a = 0)
        : r(r), g(g), b(b), a(a)
    { }
  };


} // sf ns.
#endif // SF_COLOR_HPP
