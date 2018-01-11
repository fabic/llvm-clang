#ifndef __NSTD_C_ASSERT_H
#define __NSTD_C_ASSERT_H

namespace nstd {

  extern "C" {

    void _nstd_assert_fail(
        const char *expr,
        const char *file, int line,
        const char *func
      );

    void _nstd_debug_if(
        const char *expr,
        const char *text,
        const char *file, int line,
        const char *func
      );

  } // extern "C" //


  # ifdef NDEBUG
  #   define assert(x) (void)0
  # else
  #   define assert(x) ((void)((x) || (nstd::_nstd_assert_fail(#x, __FILE__, __LINE__, __func__),0)))
  # endif


  # ifdef NDEBUG
  #   define debug_if(x, s) ((void)0)
  # else
  #   define debug_if(x, s) ((void)((x) && (_nstd_debug_if(#x, s, __FILE__, __LINE__, __func__),0)))
  # endif

} // nstd ns.

#endif // __NSTD_C_ASSERT_H
