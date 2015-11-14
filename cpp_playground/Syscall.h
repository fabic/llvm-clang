//
// Created by fabi on 11/14/15.
//

#ifndef CPP_PLAYGROUND_SYSCALL_H
#define CPP_PLAYGROUND_SYSCALL_H

namespace Std {
    class Syscall {
    public:
        static const long SYS_exit = 60;

        static inline long syscall0(long n) {
            unsigned long ret;
            __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
            return ret;
        }

        static __inline long syscall1(long n, long a1)
        {
            unsigned long ret;
            __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
            return ret;
        }
    };
}

#endif //CPP_PLAYGROUND_SYSCALL_H
