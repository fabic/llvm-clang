#include <cassert>
#include "TextBuffer.hpp"
#include "logging.hpp"

namespace sf {

  TextBuffer::TextBuffer(const filename_t filename)
  {
    _fileName = filename;
  }


  TextBuffer::~TextBuffer()
  {
    if (_fstream.is_open())
      _fstream.close();
  }


  TextBuffer::this_t
    TextBuffer::openFile()
  {
    assert( ! _fstream.is_open() );

    logdebug << "TextBuffer::openFile(): _fileName = " << _fileName ;

    _fstream.open(_fileName);

    if (! _fstream.is_open() )
      throw sf::ex::base_exception();

    int lineNb = 0;

    while( true )
    {
      lineNb++;

      Line& line = newLineAtEnd();

      std::getline(_fstream, line.string());

      logtrace << " [#" << lineNb << "]: " << line.string() ;

      if (_fstream.eof()) {
        logtrace << "TextBuffer::openFile(): Good, reached EOF." ;
        break;
      }
      else if (_fstream.fail()) {
        logwarn << "TextBuffer::openFile(): Failure occured while "
                   "reading line " << lineNb << " of file " << _fileName
                << "(continuing silently in hope this is recoverable)." ;
      }
      else if (_fstream.bad())
        throw sf::ex::base_exception();
    }

    logdebug << "TextBuffer::openFile(): Finished reading file contents, got "
             << lineNb << " lines." ;

    return this;
  }


  Line::reference_t
    TextBuffer::newLineAtEnd()
  {
    _lines.emplace_back();
    return _lines.back();
  }

} // sf ns