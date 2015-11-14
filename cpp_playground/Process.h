//
// Created by fabi on 11/14/15.
//

#ifndef CPP_PLAYGROUND_PROCESS_H
#define CPP_PLAYGROUND_PROCESS_H

#include "Syscall.h"

namespace Std {

    class Process {
    public:
        inline static void exit(int exitCode) {
            using Std::Syscall;
            Syscall::syscall1(Syscall::SYS_exit, exitCode);
        }
    };

} // Std ns.

#endif //CPP_PLAYGROUND_PROCESS_H
