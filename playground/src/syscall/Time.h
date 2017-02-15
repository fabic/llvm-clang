//
// Created by fabi on 11/14/15.
//

#ifndef FABIC_SYSCALL_TIME_H
#define FABIC_SYSCALL_TIME_H

#include "Syscall.h"

namespace kernel {

    class Time {
    public:
        // see include/linux/time.h
        struct timeval {
            long seconds;
            long microseconds;
        };

        // see include/linux/time.h
        struct timezone {
            int	minuteswest; /* minutes west of Greenwich */
            int	dsttime;     /* type of dst correction */
        };

        struct timespec {
            struct timeval  tv;
            struct timezone tz;
        };

        /**
         * @see sc_clock_gettime() "system call clock get time"
         * See musl/src/time/clock_gettime.c
         * See musl/src/time/gettimeofday.c
         */
        static inline int sc_gettimeofday(struct timeval *tv, void *tz) {
            int ec = Syscall::syscall2(
                    Syscall::SYS_gettimeofday,
                    reinterpret_cast<long>( tv ),
                    reinterpret_cast<long>( tz ) );
            return ec;
        }

        static inline struct timespec gettimeofday() {
            struct timespec ts;
            int ec = Time::sc_gettimeofday(&ts.tv, &ts.tz);
            return ts;
        }
    };

} // kernel ns.

#endif //FABIC_SYSCALL_TIME_H
