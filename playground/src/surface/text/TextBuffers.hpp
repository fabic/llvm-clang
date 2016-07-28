#ifndef SF_TEXT_BUFFERS_HPP
#define SF_TEXT_BUFFERS_HPP

#include <map>
#include <boost/filesystem.hpp>

#include "TextBuffer.hpp"

namespace sf {

  class TextBuffers {
  public:
    using TextBuffer_map_t = std::map< filename_t, TextBuffer::pointer > ;

  protected:
    TextBuffer_map_t _buffers;

  public:
    TextBuffer::pointer openFile(const filename_t filename);

  };

} // sf ns.

#endif // SF_TEXT_BUFFERS_HPP
