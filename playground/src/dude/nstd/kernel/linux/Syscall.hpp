//
// Created by fabi on 11/14/15.
//

#ifndef NSTD_KERNEL_SYSCALL_H
#define NSTD_KERNEL_SYSCALL_H

#include <nstd/types.hpp>
#include <nstd/kernel/linux/errno.hpp>

namespace kernel {

  /**
   *
   * * See `lib/musl/arch/x86_64/syscall_arch.h`
   *   for the __syscallX(...) implementations.
   * * See `lib/musl/arch/x86_64/bits/syscall.h.in`
   *   for the system call numbers.
   * * [Google's Bionic libc x86_64 sys. calls impl.](https://github.com/android/platform_bionic/tree/master/libc/arch-x86_64/syscalls)
   *
   */
  class Syscall {
  public:
    // TODO: turn this into an `enum Sys: long`
    static constexpr long SYS_read           =   0;
    static constexpr long SYS_write          =   1;
    static constexpr long SYS_open           =   2;
    static constexpr long SYS_close          =   3;

    static constexpr long SYS_brk            =  12;
    static constexpr long SYS_getpid         =  39;
    static constexpr long SYS_exit           =  60;
    static constexpr long SYS_gettimeofday   =  96;

    static constexpr long SYS_getuid         = 102;
    static constexpr long SYS_getgid         = 104;
    static constexpr long SYS_gettid         = 186;
    static constexpr long SYS_tgkill         = 234;

  public:
    /**
     * Basically returns a kernel error code from the enum `ErrorNo`
     * which is obtained by negating `r` since Linux syscalls return
     * a negative value so as to indicate an error condition.
     *
     * * NOTE: this must be invoqued only with `r` being negative!
     * * NOTE: an exception may be thrown if casting to ErrorNo fails.
     * * See `musl-libc/src/internal/syscall_ret.c`
     *
     * \return -r
     */
    static inline ErrorNo return_error_code(long r) {
      return static_cast<ErrorNo>(-r);
    }

    /// TODO: not impl.
    static void die(const char *text) __attribute((noreturn));

    static inline long syscall0(long n) {
      unsigned long ret;
      asm volatile ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
      return ret;
    }

    static inline long syscall1(long n, long a1)
    {
      unsigned long ret;
      asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
      return ret;
    }

    static inline long syscall2(long n, long a1, long a2)
    {
      unsigned long ret;
      asm volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
                              : "rcx", "r11", "memory");
      return ret;
    }

    static inline long syscall3(long n, long a1, long a2, long a3)
    {
      unsigned long ret;

      asm volatile (
          "syscall"
          : "=a"(ret)
          : "a"(n), "D"(a1), "S"(a2), "d"(a3)
          : "rcx", "r11", "memory"
        );

      return ret;
    }

#if 0
    static __inline long __syscall4(long n, long a1, long a2, long a3, long a4)
    {
      unsigned long ret;
      register long r10 __asm__("r10") = a4;
      __asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
          "d"(a3), "r"(r10): "rcx", "r11", "memory");
      return ret;
    }

    static __inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
    {
      unsigned long ret;
      register long r10 __asm__("r10") = a4;
      register long r8 __asm__("r8") = a5;
      __asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
          "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
      return ret;
    }

    static __inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
    {
      unsigned long ret;
      register long r10 __asm__("r10") = a4;
      register long r8 __asm__("r8") = a5;
      register long r9 __asm__("r9") = a6;
      __asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
          "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
      return ret;
    }
#endif // 0
  };

} // kernel ns.

#endif // NSTD_KERNEL_SYSCALL_H
