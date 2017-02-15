//
// Created by fabi on 11/14/15.
//

#ifndef FABIC_SYSCALL_PROCESS_H
#define FABIC_SYSCALL_PROCESS_H

#include "Syscall.h"

namespace kernel {


    class Process {
    public:
        typedef int pid_t;
        typedef unsigned gid_t;
        typedef unsigned uid_t;

        inline static void exit(int exitCode) {
            Syscall::syscall1(Syscall::SYS_exit, exitCode);
        }

        inline static pid_t getpid(void) {
            return static_cast<pid_t>( Syscall::syscall0(Syscall::SYS_getpid) );
        }

        inline static uid_t getuid(void) {
            return static_cast<uid_t>( Syscall::syscall0(Syscall::SYS_getuid) );
        }

        inline static gid_t getgid(void) {
            return static_cast<gid_t>( Syscall::syscall0(Syscall::SYS_getgid) );
        }

        /**
         * musl-libc/src/malloc/__brk.c
         * \link http://man7.org/linux/man-pages/man2/brk.2.html
         */
        inline static nstd::uintptr_t brk(nstd::uintptr_t newbrk = 0)
        {
            return static_cast<nstd::uintptr_t>( Syscall::syscall1(Syscall::SYS_brk, newbrk) );
        }

    };

} // kernel rs.

#endif //FABIC_SYSCALL_PROCESS_H
