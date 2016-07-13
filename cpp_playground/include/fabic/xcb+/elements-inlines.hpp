#ifndef FABIC_TK_ELEMENTS_INLINES_H
#define FABIC_TK_ELEMENTS_INLINES_H

#include "elements.hpp"

TL_NS_BEGIN
namespace tk {


  inline
  ElementList::reference
    ContainerTrait::emplaceChildLast(ElementPtr&& elt)
  {
    // TODO: C++17: return this->_children.emplace_back(elt);
    /* void */ this->_children.emplace_back(elt);
    return this->_children.back();
  }


  inline
  ElementList::reference
    ContainerTrait::appendChild(ElementPtr&& elt)
  {
    this->_children.push_back( elt );
    return this->_children.back();
  }

} // xcb ns
TL_NS_END

#endif // FABIC_TK_ELEMENTS_INLINES_H
