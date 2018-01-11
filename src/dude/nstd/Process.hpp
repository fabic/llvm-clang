#ifndef NSTD_PROCESS_H
#define NSTD_PROCESS_H

#include <nstd/kernel/linux/Process.hpp>
#include <nstd/kernel/linux/Signal.hpp>
#include <nstd/kernel/linux/Stream.hpp>
#include <nstd/String.hpp>
#include <nstd/File.hpp>

namespace nstd {

  class Process : public kernel::Process,
                  public kernel::Signal,
                  public kernel::Stream
  {
  protected:
  public:
  public:
    /** Causes abnormal process termination.
     *
     * * `lib/musl/src/exit/abort.c`
     * * `stdlib.h`
     *
     * * <http://man7.org/linux/man-pages/man3/abort.3.html>
     * * <http://en.cppreference.com/w/cpp/utility/program/abort>
     *
     * * TODO: read abort(3): we need to unblock the ABRT signal 1st
     *   before triggering it; _and_ in case it was caught or ignored,
     *   we should restore the "default disposition" for SIGABRT and
     *   attempt to raise the signal a 2nd time.
     *
     * * This function never returns -- _warning:_ an infinite loop lies there.
     */
    static void abort() __attribute__((noreturn));

  protected:
    static File _s_std_in  ;
    static File _s_std_out ;
    static File _s_std_err ;
  public:
    inline static File& StdIn()  { return _s_std_in  ; }
    inline static File& StdOut() { return _s_std_out ; }
    inline static File& StdErr() { return _s_std_err ; }
  };

} // nstd ns.

#endif // NSTD_PROCESS_H
