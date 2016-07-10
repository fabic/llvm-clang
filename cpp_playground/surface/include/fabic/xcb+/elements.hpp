#ifndef FABIC_TK_ELEMENTS_H
#define FABIC_TK_ELEMENTS_H

#include <string>

namespace fabic {
namespace tk {


using std::string;

typedef string& string_ref;

class Positionning;
typedef Positionning&  PositionningRef;

class Attributes;
typedef Attributes&    AttributesRef;


class Positionning
{
protected:
  uint64_t is_absolute:1;
  uint64_t is_relative:1;

  uint64_t is_w_pct:1, is_w_abs:1, is_w_em:1, is_w_ex:1;
  uint64_t is_h_pct:1, is_h_abs:1, is_h_em:1, is_h_ex:1;

  uint64_t is_x_pct:1, is_x_abs:1, is_x_em:1, is_x_ex:1;
  uint64_t is_y_pct:1, is_y_abs:1, is_y_em:1, is_y_ex:1;

  uint64_t is_m_pct:1, is_m_abs:1, is_m_em:1, is_m_ex:1;
  uint64_t is_p_pct:1, is_p_abs:1, is_p_em:1, is_p_ex:1;

  int16_t _x, _y;

  int16_t _width, _height;

  int16_t _margin_top,  _margin_right,  _margin_bottom,  _margin_left;
  int16_t _padding_top, _padding_right, _padding_bottom, _padding_left;

  int16_t _depth; // _offset
};



class Attributes
{
protected:
  Positionning _pos;
public:
  PositionningRef positionning() noexcept { return this->_pos; }
};


class Element;
typedef std::shared_ptr< Element > ElementPtr;
typedef std::list<ElementPtr>      ElementList;
typedef ElementList&               ElementListRef;


class ContainerTrait
{
protected:
  ElementList _children;
public:
  ElementListRef children() noexcept { return this->_children; }
}


class Element
{
protected:
  string _id;
  Attributes _attrs;

public:
  Element();
  virtual ~Element();

  Element(const Element &) = delete;
  Element& operator=(const Element &) = delete;

  const string_ref id() const noexcept { return this->_id; }

  AttributesRef attributes() noexcept { return this->_attrs; }

  /// Shortcut
  PositionningRef positionning() noexcept { return this->_attrs.positionning(); }

};


class Block
: public Element
, public ContainerTrait
{

};


class Div : public Block
{

};



} // xcb ns
} // fabic ns

#include "fabic/xcb+/elements-inline.hpp"

#endif // FABIC_TK_ELEMENTS_H
