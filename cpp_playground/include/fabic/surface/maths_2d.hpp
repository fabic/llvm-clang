#ifndef FABIC_TK_MATHS_2D_HPP
#define FABIC_TK_MATHS_2D_HPP

TL_NS_BEGIN
namespace tk {

  namespace unit {
    /// Type trait for "a pixel location" (~)
    template< typename ScalarType = int16_t >
    struct pixels {
      typedef ScalarType scalar_type;
    };
  }


/// Width x Height dimensions type
/// TODO: find another name for this "dimensions" thing.
/// TODO: have a common vs different thing for position (x,y)
/// todo: ^ like point<> / vector<> / box<> / bounding_box<>
template< class UnitTag >
struct dimensions_t
{
  typedef typename UnitTag::scalar_type  scalar_t;
  typedef       dimensions_t< UnitTag >  self_t;
  typedef                       self_t&  self_ref;

  scalar_t  width  = 0;
  scalar_t  height = 0;

  inline dimensions_t() { }

  inline dimensions_t(const dimensions_t &) = default;
  inline dimensions_t& operator=(const dimensions_t &) = default;

  inline dimensions_t(scalar_t w, scalar_t h)
      : width(w), height(h)
  { }

  /**
   * Construct w/ a box of WxH _dim_ dimensions and use default _fallback_
   * values for <= 0 width or height.
   */
  inline dimensions_t(self_t dim, self_t fallback)
      : width(  dim.width  > 0 ? dim.width  : fallback.width  ),
        height( dim.height > 0 ? dim.height : fallback.height )
  { }

  /// Set the width x height of _this_ box.
  inline self_ref set(scalar_t w, scalar_t h) noexcept
  {
    this->width  = w;
    this->height = h;
    return *this;
  }

  /**
   * Grow _this_ “ bounding box ” in width and/or height wrt. to the
   * _other_ box.
   *
   * @param other
   * @return
   */
  template< typename Other = self_t>
  self_ref grow_if_bigger(const Other other) noexcept
  {
    // this->width  = std::max(this->width,  other.width);
    // this->height = std::max(this->height, other.height);
    // ^ std::max<T>() requires the same T type for both arguments.
    this->width  = this->width  > other.width  ? this->width  : other.width;
    this->height = this->height > other.height ? this->height : other.height;
    return *this;
  }
};

/// Default signed 16-bit integer width x height dimensions type.
typedef dimensions_t< unit::pixels<> > pixels_dimensions_t;

/// Screen position
/// fixme: refactor the dimensions_t<> as a vector<> or sthg alike.
typedef dimensions_t< unit::pixels<> > pixels_position_t;



} // xcb ns
TL_NS_END

#endif //FABIC_TK_MATHS_2D_HPP
