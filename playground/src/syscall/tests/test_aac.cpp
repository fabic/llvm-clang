
#include "Process.h"

using namespace kernel;

//void _init() __attribute__((weak));
//void _fini() __attribute__((weak));
int main(int argc, char *argv[]);

/* From musl-libc: crt/x86_64/crt1.s
 * Written 2011 Nicholas J. Kain, released as Public Domain */
__asm__
(
  ".weak _init    \n"
  ".weak _fini    \n"

  ".text          \n"
  ".global _start \n"

  "_start:                \n"
  "  xor %rbp, %rbp       \n" /* rbp:undefined -> mark as zero 0 (abi) */
	"    mov %rdx, %r9      \n" /* 6th arg: ptr to register with atexit() */
	"    pop %rsi           \n" /* 2nd arg: argc */
	"    mov %rsp, %rdx     \n" /* 3rd arg: argv */
	"  andq $-16, %rsp      \n" /* align stack pointer */
	"    mov $_fini, %r8    \n" /* 5th arg: fini/dtors function */
	"    mov $_init, %rcx   \n" /* 4th arg: init/ctors function */
	"    mov $main, %rdi    \n" /* 1st arg: application entry ip */
	"  call _start_c        \n"/* musl init will run the program */

  "nop\nnop\nnop\nnop\n"
  //"nop\nnop\nnop\nnop\n"
  //"nop\nnop\nnop\nnop\n"
  //"nop\nnop\nnop\nnop\n"
);


//extern "C" void _start() __attribute__((noreturn));

/**
 * musl-libc/crt/crt1.c
 */
extern "C" _Noreturn
void _start_c(
    int (*main_)(),
    int argc, char **argv,
	  void (*init_)(),
    void (*fini_)(),
    void(*atexit_wtf)()
    )
{

  int ec = main(argc, argv);

  Process::exit(100 + ec);
}


/**
 */
int main(int argc, char *argv[])
{
    return argc;
}

