//
// Created by cadet on 7/27/16.
//

#include "Line.hpp"

namespace sf {

  bool Line::isBlank() const noexcept
  {
    if (! _dirty)
      return _is_blank_line;

    _is_blank_line = true;

    for(const int ch : _string) {
      if (! std::isblank(ch)) {
        _is_blank_line = false;
        break;
      }
    }

    return _is_blank_line;
  }

} // sf ns.