#include <nstd/c/assert.h>
#include <nstd/Process.hpp>

namespace nstd {

  /**
   * Musl-libc `src/exit/assert.c`
   *
   * TODO: See src/exit/abort.c, which raises SIGABRT and SIGKILL (then loops forever).
   * TODO: See src/exit/exit.c and src/exit/_Exit.c
   */
  void _nstd_assert_fail(const char *expr, const char *file, int line, const char *func)
  {
    //fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
    //fflush(NULL);

    if (expr != nullptr)
      Process::StdErr() << expr;

    if (file != nullptr)
      Process::StdErr() << file;

    if (func != nullptr)
      Process::StdErr() << func;

    Process::abort();
  }


  void _nstd_debug_if(
      const char *expr,
      const char *text,
      const char *file, int line,
      const char *func)
  {
    auto StdErr = Process::StdErr();

    StdErr << "debug_if("
           << "expression: " << (expr != nullptr ? expr : "<unavailable>")
           << ", text: "     << (text != nullptr ? text : "<unavailable>")
           << ", file: "     << (file != nullptr ? file : "<unavailable>")
           << ", routine: "  << (func != nullptr ? func : "<unavailable>")
           << "\n"
           ;
  }

} // nstd ns.
