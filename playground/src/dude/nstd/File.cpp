#include <nstd/File.hpp>
#include <nstd/String.hpp>
#include <nstd/c/assert.h>
#include <nstd/kernel/linux/errno.hpp>

namespace nstd {


  File::File(int fd)
    : _fd(fd)
  {}


  File::File(const char *pathName, int flags)
    : File()
      // ^ default ctor will set `_fd` to -1.
  {
    open(pathName, flags);
  }


  File::~File()
  {
    if (_fd >= 0)
      close();
  }


  File::descriptor_t
    File::open(const char *pathName, int flags, int mode)
    {
      debug_if(_fd >= 0, "File should have been closed prior to calling `open()`.");

      _fd = Stream::open(pathName, flags, mode);

      debug_if(_fd < 0, kernel::error_description(_fd));

      return _fd;
    }


  kernel::ErrorNo
    File::close()
    {
      long ec = Stream::close(_fd);
      debug_if(ec < 0, kernel::error_description(_fd));
      return kernel::Syscall::return_error_code(ec);
    }


  ssize_t
    File::write(const char *str)
    {
      assert(str != nullptr && "Null string pointer !");
      size_t len = String::strlen( str );
      return Stream::write(_fd, str, len);
    }


  ssize_t
    File::writeln(const char *str)
    {
      return write(str) + write("\n");
    }


} // nstd ns.
