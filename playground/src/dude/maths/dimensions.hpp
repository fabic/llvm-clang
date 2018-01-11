#ifndef SF_MATHS_DIMENSIONS_HPP
#define SF_MATHS_DIMENSIONS_HPP

#include "dude/maths/vector.hpp"

namespace sf {

  /// Width x Height dimensions.
  /// ( sub-classing Vector<> here only for having some
  ///   "dimensions semantics" when we code ).
  template< class Scalar = int16_t >
  class Dimensions : public Vector< Scalar > {
  public:
    using base_t = Vector< Scalar >;

  protected:
    // Hiding those two so that we speak in terms of dimensions.
    Scalar x() const noexcept;
    Scalar y() const noexcept;

  protected:
    // Alias width and heigth to (x, y) of base class.
    Scalar& width_  = base_t::x_;
    Scalar& height_ = base_t::y_;

  public:
    Dimensions() : base_t() { }

    Dimensions(Scalar width, Scalar height)
        : base_t(width, height) { }

    template< typename Other >
    Dimensions( const Dimensions< Other >& other)
        : base_t(other) { }

    Dimensions< Scalar >& operator=(const Dimensions< Scalar >& other)
    {
      this->x_ = other.x_;
      this->y_ = other.y_;
      return *this;
    }

    Scalar width()  const noexcept { return this->width_; }
    Scalar height() const noexcept { return this->height_; }
  };

} // sf ns

#endif // SF_MATHS_DIMENSIONS_HPP
