#ifndef _NSTD_KERNEL_STREAM_H
#define _NSTD_KERNEL_STREAM_H

#include <nstd/kernel/linux/Syscall.hpp>

namespace kernel {

  using nstd::ssize_t;
  using nstd::size_t;

  /** File/descriptor related syscalls.
   *
   * TODO: rename as File.
   */
  class Stream {
    public:
      /// File descriptor integral type.
      using descriptor_t = int ;

    public:
      static constexpr int STDIN  = 0;
      static constexpr int STDOUT = 1;
      static constexpr int STDERR = 2;

      struct OpenFlags {
        // `musl-libc/include/fcntl.h`
        static constexpr int RDONLY    =         0 ;
        static constexpr int WRONLY    =         1 ;
        static constexpr int RDWR      =         2 ;
        // `musl-libc/arch/x86_64/bits/fcntl.h`
        static constexpr int CREAT     =      0100 ;
        static constexpr int EXCL      =      0200 ;
        static constexpr int NOCTTY    =      0400 ;
        static constexpr int TRUNC     =     01000 ;
        static constexpr int APPEND    =     02000 ;
        static constexpr int NONBLOCK  =     04000 ;
        static constexpr int DSYNC     =    010000 ;
        static constexpr int ASYNC     =    020000 ;
        static constexpr int DIRECT    =    040000 ;
        static constexpr int DIRECTORY =   0200000 ;
        static constexpr int NOFOLLOW  =   0400000 ;
        static constexpr int NOATIME   =  01000000 ;
        static constexpr int CLOEXEC   =  02000000 ;
        static constexpr int SYNC      =  04010000 ;
        static constexpr int PATH      = 010000000 ;
        static constexpr int TMPFILE   = 020200000 ;
        static constexpr int LARGEFILE =        0 ;
        static constexpr int RSYNC     =       RSYNC ;
        static constexpr int NDELAY    =    NONBLOCK ;
        static constexpr int SEARCH    =        PATH ;
        static constexpr int EXEC      =        PATH ;
        static constexpr int ACCMODE   = 03 | SEARCH ;
      };
    public:
      static inline long open(const char *pathname, int flags, int mode = 0);
      static inline ssize_t read(int fd, void *buffer, size_t count);

      template<typename T>
        static inline ssize_t write(int fd, const T* buffer, size_t count);

      static inline long close(int fd);

      // todo: does the kernel actually returns an 'int' ?? or is it libc ?
  };


  //
  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
  //

  long
    Stream::open(const char *pathname, int flags, int mode)
    {
      return Syscall::syscall3(
          Syscall::SYS_open,
          reinterpret_cast<long>(pathname),
          flags,
          mode
        );
    }


  ssize_t
    Stream::read(descriptor_t fd, void *buffer, size_t count)
    {
      long buf = reinterpret_cast<long>( buffer );

      return Syscall::syscall3(Syscall::SYS_read, fd, buf, count);
    }


  template<typename T>
    ssize_t
      Stream::write(descriptor_t fd, const T* buffer, size_t count)
      {
        long buf = reinterpret_cast<long>( buffer );
        long size = sizeof(T) * count;
        return Syscall::syscall3(Syscall::SYS_write, fd, buf, size);
      }


  long
    Stream::close(descriptor_t fd)
    {
      return Syscall::syscall1(Syscall::SYS_close, fd);
    }


} // kernel ns.

#endif // _NSTD_KERNEL_STREAM_H
