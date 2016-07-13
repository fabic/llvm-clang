#include <iostream>

#include "fabic/syscall/Process.h"
#include "fabic/syscall/Time.h"

using namespace std;
using namespace TLNS::syscall;

void test_time_etc();

/**
 * ich bin main() !
 */
int main() {
    cout << "Hello, World!" << endl << endl;

    cout << "PID " << Process::getpid() << endl;
    cout << "UID " << Process::getuid() << endl;
    cout << "GID " << Process::getgid() << endl;

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
