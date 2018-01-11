#ifndef NSTD_EXIT_ATEXIT_H
#define NSTD_EXIT_ATEXIT_H

extern "C" {

  /**
   * Altered the original prototype `int atexit(void (*func)(void));`
   * so that it takes a `void *arg` like `__cxa_atexit`.
   */
  int atexit(void (*func)(void *), void *arg);

  /**
   * * TODO: find authoritative doc. for this one ;
   * * TODO: find out about that `void *dso` thing ;
   * * TODO: see `c/_start.c` which is handed some `void (*atexit_thing_wtf)()`
   *   function pointer, most probably some LD dyn. loader cleanup code.
   */
  int  __cxa_atexit(void (*func)(void *), void *arg, void *dso);

  void __cxa_finalize(void *dso);

}

#endif // NSTD_EXIT_ATEXIT_H
