#ifndef _DUDE_FILESYSTEM_FILE_H
#define _DUDE_FILESYSTEM_FILE_H

#include <cstdint>
#include <fstream>
#include <type_traits>
//#include <experimental/filesystem>
#include <boost/filesystem.hpp>

# include "dude/util/exceptions.hpp"

namespace dude {
namespace fs {

  //namespace xfs = std::experimental::filesystem;
  namespace xfs = boost::filesystem;

  /**
   */
  class File {
    public:
      using self       = File & ;
      using char_t     = std::fstream::char_type;
      using string_t   = std::string ;
      using string_ref = string_t & ;
    protected:
      xfs::path     _fileName;
      std::fstream  _fs;
      string_t      _content;
    public:
      explicit File(xfs::path fileName);
      virtual ~File();
      std::uintmax_t size();
      self read();
      /// Return the whole file content. File is open and read first
      /// if needed.
      string_ref content();
      /// We assume that the file was read _if_ the string buffer
      /// has characters (which is incorrect). [fixme]
      bool hasContent() const { return _content.size() > 0; }
    protected:
  };

  static_assert(
      std::is_convertible<File::string_t::value_type, char>::value,
      "Character type must be 'char'.");

  static_assert(
      std::is_convertible<std::fstream::char_type, File::char_t>::value,
      "Incorrect std::fstream character type.");

} // fs ns.
} // dude ns.

#endif // _DUDE_FILESYSTEM_FILE_H
