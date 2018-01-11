#include <nstd/Process.hpp>

namespace nstd {


  /*
   * 3 static class members for those standard file descriptors.
   */
  File Process::_s_std_in  = File( Stream::STDIN  );
  File Process::_s_std_out = File( Stream::STDOUT );
  File Process::_s_std_err = File( Stream::STDERR );


  void
    Process::abort()
    {
      pid_t tgid = getpid();
      pid_t  tid = gettid();

      long ret = tgkill(tgid, tid, SIGABRT);

      if (ret != 0)
        ; // todo: err. condition ?

      while(true) {} // noreturn (else compiler complains).
    }


} // nstd ns.

