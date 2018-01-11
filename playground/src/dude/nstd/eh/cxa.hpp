#ifndef NSTD_C_EH_CXA_H
#define NSTD_C_EH_CXA_H

#include <nstd/unwind/unwind.hpp>


// TODO: see if we can conditionally define this (for cohabitation).
namespace std {

  /**
   * * This one `std::terminate()` seems to be hard-coded somehow by Clang
   *   when generating exception handling code.
   * * <http://en.cppreference.com/w/cpp/error/terminate>
   * * `~/dev/llvm-clang/llvm-clang/libcxx/src/exception.cpp`
   */
  void terminate() noexcept __attribute__((noreturn));
  void unexpected() noexcept __attribute__((noreturn));

} // std ns.


/* ~  ~  nstd::cxxabi  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
 *
 *
 */
namespace nstd {
namespace cxxabi {

  extern "C" {

    /**
     * * `~/dev/llvm-clang/llvm-clang/libcxxabi/src/cxa_exception.cpp`
     */
    void* __cxa_begin_catch(void * unwind_arg) noexcept;

    /**
     * * `~/dev/llvm-clang/llvm-clang/libcxxabi/src/cxa_personality.cpp`
     */
    unwind::ReasonCode
      __gxx_personality_v0(unwind::UnwindState state,
                           unwind::Exception*  unwind_exception,
                           unwind::Context*    context);

  } // extern "C" //

} // cxxabi ns.
} // nstd ns.

#endif // NSTD_C_EH_CXA_H
