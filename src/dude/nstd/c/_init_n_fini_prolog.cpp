
/**
 * ## See :
 *
 * * `~$  objdump -d /usr/lib/crti.o`
 * * `~$  objdump -d /usr/lib/crtn.o`
 *
 * * /home/fabi/dev/llvm-clang/misc/gcc/libgcc/crtstuff.c
 *
 * ld --eh-frame-hdr
 *    -m elf_x86_64
 *    -shared
 *
 *    -o src/nstd/libnstd.so
 *    -soname libnstd.so
 *
 *    /usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1/../../../../lib64/crti.o
 *    /usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1/crtbeginS.o
 *
 *    -L/usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1
 *    -L/usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1/../../../../lib64
 *    -L/lib/../lib64 -L/usr/lib/../lib64
 *    -L/usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1/../../..
 *
 *    -L/home/fabi/dev/llvm-clang/local/bin/../lib -L/lib -L/usr/lib
 *
 *    src/nstd/CMakeFiles/nstd.dir/c/_start.cpp.o
 *    src/nstd/CMakeFiles/nstd.dir/c/memset.cpp.o
 *    src/nstd/CMakeFiles/nstd.dir/c/assert.cpp.o
 *    src/nstd/CMakeFiles/nstd.dir/malloc/malloc.cpp.o
 *    src/nstd/CMakeFiles/nstd.dir/String/length.cpp.o
 *    src/nstd/CMakeFiles/nstd.dir/Process.cpp.o
 *    src/nstd/CMakeFiles/nstd.dir/c/_init_n_fini_prologues.cpp.o
 *
 *    -lc++ -lc++abi -lpthread -lm -lgcc_s -lc -lgcc_s
 *
 *    /usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1/crtendS.o
 *    /usr/lib64/gcc/x86_64-pc-linux-gnu/6.3.1/../../../../lib64/crtn.o
 */

asm
(
 ".section .init  \n"
 ".global _init   \n"
 "_init:          \n"
 "  push %rax     \n"
);

asm
(
 ".section .fini  \n"
 ".global _fini   \n"
 "_fini:          \n"
 "  push %rax     \n"
);

