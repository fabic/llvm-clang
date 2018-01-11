#ifndef SF_TEXT_BUFFERS_HPP
#define SF_TEXT_BUFFERS_HPP

#include <map>
#include <boost/filesystem.hpp>

#include "TextBuffer.hpp"

namespace sf {

  /**
   * Maintains a collection of “ open ” text buffers.
   */
  class TextBuffers {
  public:
    using TextBuffer_map_t = std::map< filename_t, TextBuffer::pointer > ;

  protected:
    TextBuffer_map_t _buffers;

  public:

    /// Open file through a new TextBuffer.
    TextBuffer::pointer openFile(const filename_t filename);

    /// Return
    TextBuffer::pointer any();
  };


  // // // // // // // // // // // // // // // // // // // // // // //


  inline
  TextBuffer::pointer
    TextBuffers::any()
  {
    if (_buffers.empty())
      throw sf::ex::yet_undefined_exception("Map contains no text buffers.");

    return _buffers.begin()->second ;
  }

} // sf ns.

#endif // SF_TEXT_BUFFERS_HPP
