#include <nstd/c/atexit.hpp>

///
/// See `musl-libc/src/exit/atexit.c`
///

extern "C" {

  int atexit(void (*func)(void *), void *arg)
  {
    void *_temp_dso_thing = nullptr;
    return __cxa_atexit(func, arg, _temp_dso_thing);
  }


  int __cxa_atexit(void (*func)(void *), void *arg, void *dso)
  {
    return 0;
  }


  void __cxa_finalize(void *dso)
  {
    // Musl has no body for this one.
  }


} // "C" extern linkage.
