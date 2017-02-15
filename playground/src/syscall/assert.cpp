#include <assert.h>

/**
 * Musl-libc `src/exit/assert.c`
 */
void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
	//fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
	//fflush(NULL);
	//abort();
}
