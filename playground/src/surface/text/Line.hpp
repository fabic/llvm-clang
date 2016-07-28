#ifndef SF_TEXT_LINE_HPP
#define SF_TEXT_LINE_HPP

#include <string>

namespace sf {

  /**
   * Representation of a text buffer line.
   */
  class Line {
  public:
    using reference_t  = Line & ;
    using string_t     = std::string;

  protected:
    string_t _string;

    // _deleted / _removed

  public:

    /// Return a reference to the `std::string` used for storage of
    /// line characters.
    string_t& string() { return _string; }
  };

} // sf ns.

#endif // SF_TEXT_LINE_HPP
