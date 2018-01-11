#include <nstd/unwind/unwind.hpp>
#include <nstd/Process.hpp>

namespace nstd {
namespace unwind {

  extern "C" {

    // ~/dev/llvm-clang/misc/libunwind/src/unwind/Resume.c
    void
      _Unwind_Resume (Exception* exceptionObject)
      {
      }

  } // extern "C" //

} // unwind ns.
} // nstd ns.


namespace std {

  void terminate() // noexcept noreturn
  {
    nstd::Process::abort();
  }

  // todo: unexpected() def.

} // std ns.
