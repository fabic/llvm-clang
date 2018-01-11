
#include <nstd/types.hpp>

/**
 * TODO: impl. memset
 *
 * * GCC:`libgcc/memset.c`
 * * <http://www.xs-labs.com/en/blog/2013/08/06/optimising-memset/>
 */
extern "C"
void* memset(void* dest, int c, nstd::size_t n)
{
  auto s = reinterpret_cast<unsigned char *>( dest );

	for (; n; n--, s++) *s = c;

	return dest;
}
