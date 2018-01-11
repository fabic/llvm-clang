#ifndef NSTD_KERNEL_SIGNAL_H
#define NSTD_KERNEL_SIGNAL_H

#include <nstd/kernel/linux/Process.hpp>

namespace kernel {

  using pid_t = Process::pid_t;

  /** Process signals.
   *
   * * [Overview of signals – signal(7) man page](http://man7.org/linux/man-pages/man7/signal.7.html)
   * * `lib/musl/arch/x86_64/bits/signal.h`
   */
  class Signal {
  public:
    static constexpr long SIGHUP    =  1;
    static constexpr long SIGINT    =  2;
    static constexpr long SIGQUIT   =  3;
    static constexpr long SIGILL    =  4;
    static constexpr long SIGTRAP   =  5;
    static constexpr long SIGABRT   =  6;
    static constexpr long SIGIOT    = SIGABRT;
    static constexpr long SIGBUS    =  7;
    static constexpr long SIGFPE    =  8;
    static constexpr long SIGKILL   =  9;
    static constexpr long SIGUSR1   = 10;
    static constexpr long SIGSEGV   = 11;
    static constexpr long SIGUSR2   = 12;
    static constexpr long SIGPIPE   = 13;
    static constexpr long SIGALRM   = 14;
    static constexpr long SIGTERM   = 15;
    static constexpr long SIGSTKFLT = 16;
    static constexpr long SIGCHLD   = 17;
    static constexpr long SIGCONT   = 18;
    static constexpr long SIGSTOP   = 19;
    static constexpr long SIGTSTP   = 20;
    static constexpr long SIGTTIN   = 21;
    static constexpr long SIGTTOU   = 22;
    static constexpr long SIGURG    = 23;
    static constexpr long SIGXCPU   = 24;
    static constexpr long SIGXFSZ   = 25;
    static constexpr long SIGVTALRM = 26;
    static constexpr long SIGPROF   = 27;
    static constexpr long SIGWINCH  = 28;
    static constexpr long SIGIO     = 29;
    static constexpr long SIGPOLL   = 29;
    static constexpr long SIGPWR    = 30;
    static constexpr long SIGSYS    = 31;
    static constexpr long SIGUNUSED = SIGSYS;

  public:

    /** Sends the signal `sig` to the thread with the thread ID `tid`
     * in the thread group `tgid`.
     *
     * * `tgkill()` is Linux-specific and should not be used in programs
     *   that are intended to be portable.
     * * See the description of `CLONE_THREAD` in `clone()`
     *   for an explanation of thread groups.
     * * `int tgkill(int tgid, int tid, int sig);`
     * * <http://man7.org/linux/man-pages/man2/tgkill.2.html>
     *
     * ## Errors :
     *
     * * `EINVAL` An invalid thread ID, thread group ID, or signal was specified.
     * * `EPERM`  Permission denied.  For the required permissions, see kill(2).
     * * `ESRCH`  No  process  with	 the  specified	 thread	 ID (and thread group ID) exists.
     *
     * \return On success, zero is returned.  ~~On error, -1 is returned,
     * and `errno` is set appropriately.~~
     */
    static inline long tgkill(pid_t tgid, pid_t tid, long sig) {
      return Syscall::syscall3(Syscall::SYS_tgkill, tgid, tid, sig);
    }

  };

} // kernel ns.

#endif // NSTD_KERNEL_SIGNAL_H
