#ifndef _NSTD_C_START_H
#define _NSTD_C_START_H

void _init() __attribute__((weak));
void _fini() __attribute__((weak));

/** Invoked by the `_start` assembly code.
 *
 * * `musl-libc/crt/crt1.c`
 * * TODO: See `src/env/__libc_start_main.c`
 *
 * \param stack_top_ptr Pointer to the top of the program stack,
 * i.e. the value of `%rsp` at the beginning of `_start`;
 * hence it is a `long*` pointer to `argc`.
 *
 * \param argc the program argument count, i.e. size of the `char *argv[]`.
 *
 * \param argv an array of `char *` pointers for each program argument.
 * Note that it happens that the end of this array is also marked by
 * a `nullptr == argv[ argc ]`.
 *
 * \param atexit_wtf According to the ABI: a pointer to a function “that
 * the application shoulg register with `atexit()` – TODO: find out what that is.
 */
extern "C"
void _start_c(
    long *stack_top_ptr,
    int argc, char *argv[],
    void(*atexit_thing_wtf)()
  ) __attribute__((noreturn));


/**
 */
extern "C"
void _bootstrap_stub(
    int argc, char *argv[], char *envp[],
    decltype(_init) init,
    decltype(_fini) fini,
    void(*atexit_thing_wtf)()
  );


/**
 * The traditional `main()` C-style program entry point.
 */
extern "C" int main(int argc, char *argv[], char *env[]) __attribute__((weak));


/** `_start()` program low-level entry point, invoques _start_c(),
 * which in turn invoques main().
 *
 * * Originally borrowed from musl-libc: `crt/x86_64/crt1.s`
 *   (written 2011 Nicholas J. Kain, released as Public Domain).
 * * which was dropped at release v1.1.13 (Feb. 2016) and replaced
 *   by `lib/musl/crt/crt1.c` & `lib/musl/arch/x86_64/crt_arch.h`
 * * See Musl-libc commit c5e34dabbb47d8e97a4deccbb421e0cd93c0094b (2013)
 *   “new mostly-C crt1 implementation” ;
 * * and commit 6fef8cafbd0f6f185897bc87feb1ff66e2e204e1 (2015).
 *   “remove hand-written crt1.s and Scrt1.s files for all archs”
 *
 * ## From [the AMD64 x86-64-abi-0.99.pdf (page 29, 30)](http://refspecs.linuxbase.org/elf/x86-64-abi-0.99.pdf) :
 *
 * * __%rsp__ : The stack pointer holds the address of the byte with lowest address
 *              which is part of the stack. It is guaranteed to be 16-byte aligned
 *              at process entry.
 * * __%rbp__ : The content of this register is unspecified at process initialization
 *              time, but the user code should mark the deepest stack frame by setting
 *              the frame pointer to zero.
 * * __%rdx__ : A function pointer that the application should register with `atexit()`.
 *
 * ### Initial process stack :
 *
 *     +-----------------+-----------------------------------------------------
 *     | Start address : | Description :
 *     |-----------------+-----------------------------------------------------
 *     |                 | (^ high addresses)
 *     |                 | Information block, including argument strings,
 *     |                 | environment strings, auxiliary information, etc...
 *     | <UNSPECIFIED>   |
 *     |                 | NULL (end of auxiliary vector entries).
 *     |                 | Auxiliary vector entries ...
 *     |                 | NULL (end of environment pointers).
 *     |                 | Environment pointers ...
 *     | %rsp+8 + 8*argc | NULL marker (end of argv[])
 *     | ...             |
 *     | %rsp+8          | char *argv[] | Argument pointers ...
 *     | %rsp            | argc         | Argument count
 *     |                 | <PROCESS STACK TOP>
 *     |                 | (low addresses)
 *     +-----------------+-----------------------------------------------------
 *
 * ## Post scriptum
 *
 * * See `x86-64-abi-0.99.pdf` page 17,
 *   “§3.2.3 – Function Calling Reference – Parameter Passing” :
 *   function arguments (of class integer) are passed to callees
 *   in this order : %rdi, %rsi, %rdx, %rcx, %r8, %r9.
 */
extern "C" void _start(void) __attribute__((noreturn));

#endif // _NSTD_C_START_H

