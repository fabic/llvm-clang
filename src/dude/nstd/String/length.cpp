
#include <nstd/String.hpp>
#include <nstd/c/assert.h>

namespace nstd {

  using nstd::size_t;

  /**
   * <pre>
   * ~$ python
   *
   * # "MAGIC" MASK
   * > m=0x7F7F7F7F
   *
   * # Word `x` has no zero byte.
   *
   * > x=0x79efac33 ;  y=(x&m)+m; z=~(y|x|m); 'x={0:#x} | y={1:#x} | z={2:#x}'.format(x,y,z)
   *  'x=0x79efac33 | y=0xf8eeabb2 | z=-0x100000000'
   *                                       ~~~~~~~~ 4 bytes of word turned zero.
   *                                      ^ overflow.
   *
   * # Else the 1st least-significant byte that is zero
   * # turn 0x80 with leading bytes at 0x00.
   *
   * > x=0x00efac33 ; y=(x&m)+m; z=~(y|x|m); 'x={0:#x} |  y={1:#x} | z={2:#x}'.format(x,y,z)
   * 'x=0x00efac33 | y=0x7feeabb2 | z=-0x80000000'
   *
   * > x=0x7900ac33 ; y=(x&m)+m; z=~(y|x|m); 'x={0:#x} | y={1:#x} | z={2:#x}'.format(x,y,z)
   * 'x=0x7900ac33 | y=0xf87fabb2 | z=-0xff800000'
   *
   * > x=0x79ef0033 ; y=(x&m)+m; z=~(y|x|m); 'x={0:#x} | y={1:#x} | z={2:#x}'.format(x,y,z)
   * 'x=0x79ef0033 | y=0xf8ee7fb2 | z=-0xffff8000'
   *
   * > x=0x79efac00 ; y=(x&m)+m; z=~(y|x|m); 'x={0:#x} | y={1:#x} | z={2:#x}'.format(x,y,z)
   * 'x=0x79efac00 | y=0xf8eeab7f | z=-0xffffff80'
   * </pre>
   */
  inline bool word_has_zero(unsigned long x)
  {
    using word_t = decltype(x);

    constexpr word_t m = 0x7F7F7F7FL | (0x7F7F7F7FL << 32);

    word_t y = (x & m) + m;

    word_t z = ~(y | x | m);

    return z != 0;
  }

  /**
   * * `musl-libc:src/string/strlen.c`
   * * https://github.com/android/platform_bionic/tree/master/libc/arch-x86_64/string
   * * http://stackoverflow.com/a/24805807/643087
   */
  size_t String::strlen(const char *s)
  {
    assert( s != nullptr );

    using word_t = const unsigned long;

    constexpr size_t Alignment = sizeof(word_t);

    const char *a = s;

    for (; reinterpret_cast<word_t>(s) % Alignment; s++) {
      if (!*s)
        return s-a;
    }

    word_t *w = reinterpret_cast<word_t *>( s );

    for (; !word_has_zero(*w); w++) {}

    s = reinterpret_cast<const char *>(w);

    for (; *s; s++) {}

    return s-a;
  }

} // nstd ns.
