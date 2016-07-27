//
// Created by fabi on 11/14/15.
//

#ifndef FABIC_SYSCALL_PROCESS_H
#define FABIC_SYSCALL_PROCESS_H

#include "syscall/Syscall.h"

TL_NS_BEGIN
namespace syscall {

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
    };

} // syscall ns.
TL_NS_END

#endif //FABIC_SYSCALL_PROCESS_H
