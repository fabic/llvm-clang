#ifndef SF_MATHS_RECTANGLE_HPP
#define SF_MATHS_RECTANGLE_HPP

#include <c++/v1/type_traits>
#include "fabic/surface/maths/dimensions.hpp"

namespace sf {

  /// A 2D Rectangle.
  template< class Scalar = int16_t >
  class Rectangle {
  public:
    typedef Vector< Scalar >     vector_t;
    typedef Dimensions< Scalar > dimensions_t;
    typedef vector_t             position_t; ///< for code readability, alledgedly.

  protected:
    vector_t     position_;
    dimensions_t dimensions_;

  public:
    /// Default ctor.
    Rectangle()
        : position_(0, 0), dimensions_(0, 0)
    {  }

    /// Construct a rectangle at position `pos` with dimensions `dim`.
    Rectangle(vector_t pos, dimensions_t dim)
        : position_( pos ), dimensions_( dim ) { }

    vector_t     position()   const noexcept { return this->position_; }
    dimensions_t dimensions() const noexcept { return this->dimensions_; }
  };


  // Just to remind us that a dimensions is basically an alias for a vector.
  static_assert(
      std::is_same<
          Rectangle<>::vector_t,
          Rectangle<>::dimensions_t::base_t // i.e. base vector<> class (of the same scalar type).
        >::value
  );

} // sf ns

#endif //SF_MATHS_RECTANGLE_HPP
