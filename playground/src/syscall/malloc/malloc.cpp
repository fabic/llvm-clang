
#include <malloc/malloc.h>

/**
 * `[...] undefined reference to 'operator new(unsigned long)'.`
 * See `llvm-clang/libcxx/include/new
 */
void* operator new(nstd::size_t sz)
{
  return nullptr;
}
