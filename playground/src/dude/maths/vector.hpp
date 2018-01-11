#ifndef SF_MATHS_VECTOR_HPP
#define SF_MATHS_VECTOR_HPP

#include <cstdint>

namespace sf {

  /// A 2D vector.
  template< class Scalar = int16_t >
  class Vector {
  protected:
    Scalar x_ = 0;
    Scalar y_ = 0;
  public:
    Vector() { }

    Vector(Scalar x, Scalar y) : x_(x), y_(y) { }

    template< typename Other >
    Vector( const Vector< Other >& other )
        : x_(other.x()), y_(other.y()) { }

    Scalar x() const noexcept { return this->x_; }
    Scalar y() const noexcept { return this->y_; }
  };

} // sf ns

#endif // SF_MATHS_VECTOR_HPP
