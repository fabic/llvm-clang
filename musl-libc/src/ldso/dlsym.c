#include <dlfcn.h>

void *__dlsym(void *restrict, const char *restrict, void *restrict);

void *dlsym(void *restrict p, const char *restrict s)
{
	return __dlsym(p, s, 0);
}

// FabiC/2014-09-17 //
void *dlvsym(void *restrict p,
	         const char *restrict s,
	         const char *restrict v)
{
	//return __dlsym(p, s, v);
	return __dlsym(p, s, 0);
	//                   ^ can't tell if this is the version arg.as i can't get
	//                     to the definition of that __dlsym symbol -__-
}
