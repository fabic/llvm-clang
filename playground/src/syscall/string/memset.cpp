
#include <types.h>

extern "C"
void* memset(void* dest, int c, nstd::size_t n)
{
  auto s = reinterpret_cast<unsigned char *>( dest );

	/* Pure C fallback with no aliasing violations. */
	for (; n; n--, s++) *s = c;

	return dest;
}
