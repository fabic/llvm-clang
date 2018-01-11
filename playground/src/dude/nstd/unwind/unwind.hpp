#ifndef NSTD_C_EH_UNWIND_H
#define NSTD_C_EH_UNWIND_H

#include <nstd/types.hpp>

/*
 * * `clang/lib/Headers/unwind.h`
 * * `libunwind/include/unwind.h`
 */

namespace nstd {
namespace unwind {

  /** `_Unwind_State`
   *
   * For the 1st argument of `__gxx_personality_v0()`, whose decl. was
   * taken from `libcxxabi/src/cxa_personality.cpp`
   *
   * FIXME: find out what that arg. actually is.
   *
   * Couldn't find this type from libunwind, see :
   * * `clang/lib/Headers/unwind.h`
   * * `compiler-rt/lib/builtins/unwind-ehabi-helpers.h`
   */
  using UnwindState = uint32_t ;


  /** `_Unwind_Context`
   *
   * Opaque data-type from the exception handling code perspective.
   *
   * * TODO: impl. -- serves here as forward decl.
   * * `~/dev/llvm-clang/misc/libunwind/src/unwind/unwind-internal.h`
   */
  struct Context;


  /**
   */
  enum ReasonCode // _Unwind_Reason_Code
  {
    URC_NO_REASON                = 0,
    URC_FOREIGN_EXCEPTION_CAUGHT = 1,
    URC_FATAL_PHASE2_ERROR       = 2,
    URC_FATAL_PHASE1_ERROR       = 3,
    URC_NORMAL_STOP              = 4,
    URC_END_OF_STACK             = 5,
    URC_HANDLER_FOUND            = 6,
    URC_INSTALL_CONTEXT          = 7,
    URC_CONTINUE_UNWIND          = 8
  } ;


  /** `_Unwind_Exception`
   */
  struct Exception; // forward decl.

  typedef void (*ExceptionCleanupFn)( ReasonCode,
                                         Exception* );


  /**
   * “The C++ ABI requires exception_class, private_1, and private_2 to
   * be of type uint64 and the entire structure to be
   * double-word-aligned. [...]”
   */
  struct _Unwind_Exception
  {
    uint64_t            exceptionClass;
    ExceptionCleanupFn  exceptionCleanup;
    uint64_t            private_1;
    uint64_t            private_2;
  } __attribute__((__aligned__)) ;


  extern "C" {

    /**
     */
    void _Unwind_Resume(Exception*);

  } // extern "C" //

} // unwind ns.
} // nstd ns.

#endif // NSTD_C_EH_UNWIND_H
