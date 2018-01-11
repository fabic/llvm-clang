//
// Created by fabi on 11/14/15.
//

#ifndef NSTD_KERNEL_PROCESS_H
#define NSTD_KERNEL_PROCESS_H

#include "nstd/kernel/linux/Syscall.hpp"

namespace kernel {

  using nstd::uintptr_t;

  class Process {
  public:
    typedef int pid_t;
    typedef unsigned gid_t;
    typedef unsigned uid_t;

    inline static void exit(int exitCode) {
      Syscall::syscall1(Syscall::SYS_exit, exitCode);
    }

    inline static pid_t getpid(void) {
      return static_cast<pid_t>( Syscall::syscall0(Syscall::SYS_getpid) );
    }

    inline static uid_t getuid(void) {
      return static_cast<uid_t>( Syscall::syscall0(Syscall::SYS_getuid) );
    }

    inline static gid_t getgid(void) {
      return static_cast<gid_t>( Syscall::syscall0(Syscall::SYS_getgid) );
    }

    /** Get thread identification.
     *
     * * `pid_t gettid(void);`  (`sys/types.h`)
     * * <http://man7.org/linux/man-pages/man2/gettid.2.html>
     * * Note: There is no glibc wrapper for this system call.
     * * `gettid()` returns the caller's __thread ID (TID)__.
     * * In a single-threaded process, the thread ID is equal to the
     *   process ID (PID, as returned by `getpid()`).
     * * In a multithreaded process, all threads have the _same PID_,
     *   but each one has a __unique TID__.
     * * For further details, see the discussion of `CLONE_THREAD`
     *   in `clone()`.
     * * __ERRORS:__ This call is always successful.
     * * __CONFORMING TO:__ `gettid()` is Linux-specific and should not be
     *   used in programs that are intended to be portable.
     * * __NOTES:__ The thread ID returned by this call is not the same
     *   thing as a POSIX thread ID (i.e., the opaque value returned by
     *   `pthread_self()`).)
     *
     * \return The thread ID of the calling process.
     */
    inline static pid_t gettid(void) {
      return static_cast<pid_t>( Syscall::syscall0(Syscall::SYS_gettid) );
    }

    /**
     * * `musl-libc/src/malloc/__brk.c`
     * * <http://man7.org/linux/man-pages/man2/brk.2.html>
     */
    inline static uintptr_t brk(uintptr_t newbrk = nullptr)
    {
      return reinterpret_cast<uintptr_t>(
          Syscall::syscall1(Syscall::SYS_brk, (long)newbrk));
    }

  };

} // kernel ns.

#endif // NSTD_KERNEL_PROCESS_H
