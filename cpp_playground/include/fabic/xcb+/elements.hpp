#ifndef FABIC_TK_ELEMENTS_H
#define FABIC_TK_ELEMENTS_H

#include <string>
#include <list>
#include <memory>
#include <type_traits>
#include <algorithm>

# include "fabic/util/pointers.hpp"
# include "fabic/logging.hpp"

namespace fabic {
namespace tk {

using fabic::ptr::inheritable_shared_from_this;

using std::string;

typedef string&       string_ref;
typedef const string& string_cref;

class Positionning;
typedef Positionning&  PositionningRef;
typedef Positionning*  PositionningPtr;

class Attributes;
typedef Attributes&    AttributesRef;
typedef Attributes*    AttributesPtr;


enum Placement : uint8_t {
  NA = 0 // “ doesn't apply ”
  , AUTO  = 1 // equiv. "document flow" somehow ?
  , FIXED = 2
  , LEFT, RIGHT
  , TOP,  BOTTOM
  , TOP_LEFT,    TOP_RIGHT
  , BOTTOM_LEFT, BOTTOM_RIGHT
};


enum Unit : uint8_t {
  PIXELS = 0
  , PERCENTAGE
  , FONT_EM
  , FONT_EX
};


namespace unit {
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


/**
 *
 */
class Positionning
{
public:
  typedef Positionning& self;
  typedef Positionning* self_ptr;

protected:
  uint64_t _type:2; // absolute, relative, ??
  uint64_t _placement:4;

  // uint64_t is_absolute:1;
  // uint64_t is_relative:1;
  // ^ position attr. ?

  int16_t _x = -1,
          _y = -1
          ;

  pixels_dimensions_t _dimensions;

  int16_t _width  = -1,
          _height = -1;

  int16_t _top    = -1,
          _left   = -1,
          _right  = -1,
          _bottom = -1
          ;

  int16_t _x_unit:3,
          _y_unit:3,
          _w_unit:3,
          _h_unit:3
          ;

  int16_t _min_width  = -1,
          _max_width  = -1,
          _min_height = -1,
          _max_height = -1
          ;

  int16_t _margin_top,  _margin_right,  _margin_bottom,  _margin_left;
  int16_t _padding_top, _padding_right, _padding_bottom, _padding_left;

  int16_t _offset = 0; // ~depth

public:
  Positionning()
    : _placement( Placement::AUTO )
    , _dimensions(-1, -1)
    , _x_unit( Unit::PIXELS )
    , _y_unit( Unit::PIXELS )
    , _w_unit( Unit::PIXELS )
    , _h_unit( Unit::PIXELS )
  { }

  inline self xy(
      int16_t x, int16_t y,
      Unit ux = Unit::PIXELS,
      Unit uy = Unit::PIXELS
    ) noexcept
  {
    this->_x = x;
    this->_x_unit = ux;

    this->_y = y;
    this->_y_unit = uy;

    return *this;
  }

  inline self_ptr dimensions(int16_t w, int16_t h) noexcept
  {
    //this->_dimensions.set(w, h);
    this->_dimensions = pixels_dimensions_t(w, h);
    return this;
  }

  inline pixels_dimensions_t dimensions() const noexcept
  {
    return this->_dimensions;
  }

  inline uint8_t placement() const noexcept { return this->_placement; }

  inline self_ptr placement(Placement p) noexcept
  {
    this->_placement = p;

    switch( this->_placement ) {
      case Placement::BOTTOM :
        this->_bottom = 0;
        this->_top    = -1;
        break;
      default:
        ;
    }

    return this;
  }
};



class Attributes
{
protected:
  Positionning _pos;
public:
  PositionningPtr positionning() noexcept { return &this->_pos; }
};


class Element;
typedef std::shared_ptr< Element > ElementPtr;
typedef std::list< ElementPtr >    ElementList;
typedef ElementList&               ElementListRef;


class ContainerTrait
{
protected:
  ElementPtr  parent_;
  ElementList _children;
public:
  ContainerTrait(ElementPtr parent_);
  virtual ~ContainerTrait();

  ElementPtr parent() noexcept { return this->parent_; }
  ElementListRef children() noexcept { return this->_children; }
  bool hasChildren() const noexcept { return !this->_children.empty(); }

  ElementList::reference // i.e. ElementPtr& => shared_ptr<...>&
    emplaceChildLast(ElementPtr&& elt);

  ElementList::reference // i.e. ElementPtr& => shared_ptr<...>&
    appendChild(ElementPtr&& elt);

};


/**
 * @brief Base class for HTML-like elements.
 *
 * * MUST NOT: resort to std::make_shared< Concrete >(...) for instanciating
 *             or any sub-type due to the inheritable_shared_from_this<...>
 *             base class magic.
 */
class Element
  : public ContainerTrait
  , public inheritable_shared_from_this< Element >
    // public std::enable_shared_from_this< Element >
{
public:
  typedef Element& self;
  typedef Element* self_ptr;

protected:
  string     _id;
  Attributes _attributes;
  Attributes _computedAttributes;

public:
  explicit Element(ElementPtr parent_);

  virtual ~Element() override ;

  Element(const Element &) = delete;
  Element& operator=(const Element &) = delete;

  self_ptr id(string&& id) noexcept
  {
    this->_id = std::move(id);
    return this;
  };

  string_cref id() const noexcept { return this->_id; }

  AttributesPtr attributes() noexcept { return &this->_attributes; }

  virtual pixels_dimensions_t preComputePositionning(
      int16_t w, int16_t h,
      int16_t x, int16_t y
    );
  // todo: compute & postComputePositionning()...

protected:
  virtual void _initChildrenElementsHierarchy();
};


class Block
  : public Element
{
public:
  Block(ElementPtr parent_);
  virtual ~Block() override ;
};


class Div
  : public Block
{
  // Div(ElementPtr parent_);
  // virtual ~Div() override ;

};



} // xcb ns
} // fabic ns

#endif // FABIC_TK_ELEMENTS_H
