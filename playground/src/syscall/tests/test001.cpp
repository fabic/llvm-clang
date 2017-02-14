#include <iostream>

#include "syscall/Process.h"
#include "syscall/Time.h"

using namespace std;
using namespace kernel;

void test_time_etc();

/**
 * ich bin main() !
 */
int main() {
    cout << "Hello, World!" << endl << endl;

    cout << "PID " << Process::getpid() << endl;
    cout << "UID " << Process::getuid() << endl;
    cout << "GID " << Process::getgid() << endl;

    cout << "brk() " << Process::brk() << endl;

    auto newbrk = Process::brk() + 16384;
    cout << "brk() " << Process::brk(newbrk) << endl;

    test_time_etc();

    Process::exit(121);

    cout << "Bye, World! (and you should not see this BTW)" << endl;

    return 0;
}

void test_time_etc() {
    struct Time::timespec ts = Time::gettimeofday();
    cout << "Seconds      : " << ts.tv.seconds << endl;
    cout << "Microseconds : " << ts.tv.microseconds << endl;
}
