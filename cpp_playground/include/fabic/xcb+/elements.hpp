#ifndef FABIC_TK_ELEMENTS_H
#define FABIC_TK_ELEMENTS_H

#include <string>
#include <list>
#include <memory>

# include "fabic/util/pointers.hpp"

namespace fabic {
namespace tk {

using fabic::ptr::inheritable_shared_from_this;

using std::string;

typedef string&       string_ref;
typedef const string& string_cref;

class Positionning;
typedef Positionning&  PositionningRef;

class Attributes;
typedef Attributes&    AttributesRef;


class Positionning
{
public:
  enum Placement {
    LEFT, RIGHT, TOP, BOTTOM
  };
protected:
  uint64_t type:2; // absolute, relative, ??
  uint64_t placement:3; // top, right, bottom, left, top-right, top-left, etc...

  // uint64_t is_absolute:1;
  // uint64_t is_relative:1;

  uint64_t is_w_pct:1, is_w_abs:1, is_w_em:1, is_w_ex:1;
  uint64_t is_h_pct:1, is_h_abs:1, is_h_em:1, is_h_ex:1;

  uint64_t is_x_pct:1, is_x_abs:1, is_x_em:1, is_x_ex:1;
  uint64_t is_y_pct:1, is_y_abs:1, is_y_em:1, is_y_ex:1;

  uint64_t is_m_pct:1, is_m_abs:1, is_m_em:1, is_m_ex:1;
  uint64_t is_p_pct:1, is_p_abs:1, is_p_em:1, is_p_ex:1;

  int16_t _x, _y;

  int16_t _width, _height;

  int16_t _min_width,  _max_width;
  int16_t _min_height, _max_height;

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


class Element
  : public ContainerTrait
  , public inheritable_shared_from_this< Element >
    // public std::enable_shared_from_this< Element >
{
protected:
  string     _id;
  Attributes _attributes;

public:
  explicit Element(ElementPtr parent_);

  virtual ~Element() override ;

  Element(const Element &) = delete;
  Element& operator=(const Element &) = delete;

  string_cref id() const noexcept { return this->_id; }

  AttributesRef attributes() noexcept { return this->_attributes; }

  /// Shortcut
  PositionningRef positionning() noexcept { return this->_attributes.positionning(); }

  virtual ElementList preComputePositionning(
      int16_t w, int16_t h,
      int16_t x, int16_t y
    );

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
