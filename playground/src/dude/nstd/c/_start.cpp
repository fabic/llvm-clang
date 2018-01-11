
#include <nstd/c/_start.h>
#include <nstd/c/assert.h>
#include <nstd/Process.hpp>

using nstd::Process;

/* See `` c/_start.h `` for some documentation. */

asm
(
  ".text                  \n"
  ".global _start         \n"
  ".p2align 8, 0x90       \n" // admittedly useless -_- but I'm a noob for now.
  ".type _start,@function \n"

  "_start:                \n"
  "  xor %rbp, %rbp       \n" /* rbp:undefined -> mark as zero 0 (abi) */
  "    mov %rsp, %rdi     \n" /* 1st arg: pointer to top of stack. */
  "    mov %rdx, %rcx     \n" /* 4th arg: ptr to register with atexit() */
  "    mov  (%rsp), %rsi  \n" /* 2nd arg: argc */
  "    lea 8(%rsp), %rdx  \n" /* 3rd arg: argv */
  "  andq $-16, %rsp      \n" /* align stack pointer */
  "  call _start_c        \n" /* hand over execution to _start_c */

  "  hlt                  \n"

  "nop\nnop\nnop\nnop\n" /* padding, alignment */
);


void _start_c(
    long *stack_top_ptr,
    int argc, char *argv[],
    void(*atexit_thing_wtf)()
    )
{
# ifndef NDEBUG
  /* For information: argc & argv may simply have been inferred here,
   * instead of being setup by the _start assembly. */
  {
    int    _argc = static_cast<int>( *stack_top_ptr );
    char **_argv = reinterpret_cast<char**>( stack_top_ptr + 1 );
    assert(_argc == argc && _argv == argv);
  }
# endif // NDEBUG

  assert( argv[argc] == nullptr );

  char **envp = &argv[ argc + 1 ];

  _bootstrap_stub(
     argc, argv, envp,
     _init, _fini, atexit_thing_wtf );

  if (main != nullptr) {
    int ec = main(argc, argv, envp);
    Process::exit( ec );
  }
  else {
    Process::StdErr() << "_start_c(): C-style `main()` program entry point is undefined ?!";
    Process::abort();
  }

  // TODO: See src/exit/atexit.c: __cxa_atexit(), __cxa_finalize() and atexit().

  // Unreachable; prevents Clang from generating a warning
  Process::abort();
}


void _bootstrap_stub(
    int argc, char *argv[], char *envp[],
    decltype(_init) init,
    decltype(_fini) fini,
    void(*atexit_thing_wtf)()
  )
{
}

