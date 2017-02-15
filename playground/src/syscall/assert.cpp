#include <assert.h>

/**
 * Musl-libc `src/exit/assert.c`
 *
 * TODO: See src/exit/abort.c, which raises SIGABRT and SIGKILL (then loops forever).
 * TODO: See src/exit/exit.c and src/exit/_Exit.c
 */
void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
	//fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
	//fflush(NULL);
	//abort();
}
