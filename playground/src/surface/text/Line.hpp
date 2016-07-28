#ifndef SF_TEXT_LINE_HPP
#define SF_TEXT_LINE_HPP

#include <string>
#include <cctype>

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

    bool _dirty = true;
    mutable bool _is_blank_line = true;

    // _deleted / _removed

  public:

    /// Return a reference to the `std::string` used for storage of
    /// line characters.
    string_t& string() { return _string; }

    bool isDirty() const noexcept ;
    bool isBlank() const noexcept ;
  };


  // // // // // // // // // // // // // // // // // // // // // // //


  inline bool Line::isDirty() const noexcept { return _dirty ; }

} // sf ns.

#endif // SF_TEXT_LINE_HPP
