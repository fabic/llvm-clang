#ifndef FABIC_TK_ELEMENTS_INLINES_H
#define FABIC_TK_ELEMENTS_INLINES_H

#include "elements.hpp"

namespace fabic {
namespace tk {


  inline
  ElementList::reference
    ContainerTrait::emplaceChildLast(ElementPtr&& elt)
  {
    // TODO: C++17: return this->_children.emplace_back(elt);
    /* void */ this->_children.emplace_back(elt);
    return this->_children.back();
  }

} // xcb ns
} // fabic ns

#endif // FABIC_TK_ELEMENTS_INLINES_H
