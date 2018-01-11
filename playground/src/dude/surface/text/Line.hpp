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

    struct RenderStates {
      double screen_x = 0 ,
             screen_y = 0 ;

      using self_ref_t = RenderStates & ;

      /// Set this line's screen (X,Y) bottom-left coordinates.
      self_ref_t position(double x, double y) noexcept {
        screen_x = x;
        screen_y = y;
        return *this;
      }
    } ;

  protected:
    string_t _string;

    RenderStates _renderStates ;

    bool _dirty = true;
    mutable bool _is_blank_line = true;

    // _deleted / _removed

  public:

    /// Return a reference to the `std::string` used for storage of
    /// line characters.
    string_t& string() { return _string; }

    RenderStates& renderStates() noexcept { return _renderStates; }

    bool isDirty() const noexcept ;
    bool isBlank() const noexcept ;
  };


  // // // // // // // // // // // // // // // // // // // // // // //


  inline bool Line::isDirty() const noexcept { return _dirty ; }

} // sf ns.

#endif // SF_TEXT_LINE_HPP
