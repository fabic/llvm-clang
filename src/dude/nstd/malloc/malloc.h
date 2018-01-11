#ifndef __NSTD_MALLOC_H
#define __NSTD_MALLOC_H

#include <nstd/types.hpp>

// todo: throw bad_alloc.

/*
 * * https://github.com/android/platform_bionic/blob/master/libstdc%2B%2B/include/new
 */

void* operator new(nstd::size_t sz);

void* operator new[](nstd::size_t sz);

#endif //__NSTD_MALLOC_H
