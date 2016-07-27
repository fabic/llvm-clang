#ifndef SF_UI_ELEMENTS_INLINES_H
#define SF_UI_ELEMENTS_INLINES_H

#include "elements.hpp"

namespace sf {
namespace ui {


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

} // ui ns
} // sf ns

#endif // SF_UI_ELEMENTS_INLINES_H
