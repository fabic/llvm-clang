#include "dude/filesystem/file.hpp"
#include "dude/util/logging.hpp"

namespace dude {
namespace fs {

  // ctor
  File::File(xfs::path fileName) : _fileName(fileName) {}


  // dtor
  File::~File()
  {
  }


  std::uintmax_t
    File::size()
    {
      return xfs::file_size(_fileName);
    }


  File::string_ref
    File::content()
    {
      // Read the whole file first if needed.
      if (! hasContent()) {
        read();
      }

      return _content;
    }


  File::self
    File::read()
    {
      auto fileSize = size();

      logtrace << "Reading whole content of file: " << _fileName
               << ", size: " << fileSize;

      // We do close the file, and re-open it for reading [absurd?].
      if (_fs.is_open()) {
        _fs.close();
        logtrace << "Closed previously opened file.";
      }

      // Try open.
      _fs.open(_fileName.c_str(), std::ios_base::in);
      if (_fs.fail())
        throw dude::ex::yet_undefined_exception("Failed to open file.");

      _content.resize(fileSize, '#');

      _fs.read(
          const_cast<char*>(_content.data()), // Huh huh !
          _content.capacity() );

      auto readCount = _fs.gcount();
      if (readCount != fileSize) {
        logwarn << "Beware: read less bytes than the file size: "
                << readCount << " <> " << fileSize;
      }

      // Try to read one more character so as to reach the EOF stream state.
      auto ch = _fs.get();
      if (! _fs.eof()) {
        logwarn << "Didn't reach the end of file ?";
        _fs.putback(ch);
      }

      logtrace << "Finished reading whole content of file: " << _fileName;
      // logtrace << "Content :\n" << _content << "`- - - - - - - -";

      return *this;
    }

} // fs ns.
} // dude ns.
