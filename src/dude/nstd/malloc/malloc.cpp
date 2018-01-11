
#include <nstd/malloc/malloc.h>
#include <nstd/kernel/linux/Process.hpp>

using kernel::Process;
using nstd::uintptr_t;
using nstd::size_t;

/**
 * `[...] undefined reference to 'operator new(unsigned long)'.`
 * See `llvm-clang/libcxx/include/new`
 */
void* operator new(size_t sz)
{
  uintptr_t ptr = Process::brk();
  ptr += sz;
  Process::brk(ptr);
  return ptr;
}

void* operator new[](nstd::size_t sz)
{
  return ::operator new( sz );
}
