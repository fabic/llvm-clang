//
// Created by fabi on 11/14/15.
//

#ifndef FABIC_SYSCALL_SYSCALL_H
#define FABIC_SYSCALL_SYSCALL_H

TL_NS_BEGIN
namespace syscall {

    /**
     * see musl/arch/x86_64/syscall_arch.h
     *
     * @author fabic
     */
    class Syscall {
    public:
        // see musl/include/bits/syscall.h
        static const long SYS_getpid         =  39;
        static const long SYS_exit           =  60;
        static const long SYS_gettimeofday   =  96;
        static const long SYS_getuid         = 102;
        static const long SYS_getgid         = 104;

        static inline long syscall0(long n) {
            unsigned long ret;
            __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
            return ret;
        }

        static inline long syscall1(long n, long a1)
        {
            unsigned long ret;
            __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
            return ret;
        }

        static inline long syscall2(long n, long a1, long a2)
        {
            unsigned long ret;
            __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
                                  : "rcx", "r11", "memory");
            return ret;
        }
    };

} // syscall ns.
TL_NS_END

#endif //FABIC_SYSCALL_SYSCALL_H
