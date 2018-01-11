#ifndef SF_TEXT_BUFFER_HPP
#define SF_TEXT_BUFFER_HPP

#include <memory>
#include <fstream>
#include <list>

#include "surface/common.hpp"
#include "surface/text/Line.hpp"

namespace sf {

  using filename_t = std::string ;


  /**
   * In-memory representation of a text file, read line-by-line.
   */
  class TextBuffer {
  public:
    using this_t       = TextBuffer * ;
    using reference_t  = TextBuffer & ;
    using pointer      = std::shared_ptr< TextBuffer >;
    using lines_list_t = std::list< Line > ;

  protected:
    filename_t    _fileName;
    std::fstream  _fstream;
    lines_list_t  _lines;

  public:
    explicit TextBuffer(const filename_t filename);

    virtual ~TextBuffer();

    this_t openFile();

    /// Add new line at end of buffer.
    Line::reference_t newLineAtEnd();

    /// \return a reference to the Line-s list.
    lines_list_t& lines();
  };


  // // // // // // // // // // // // // // // // // // // // // // //


  inline
  TextBuffer::lines_list_t &
    TextBuffer::lines()
  {
    return _lines;
  }

} // sf ns.
#endif // SF_TEXT_BUFFER_HPP
