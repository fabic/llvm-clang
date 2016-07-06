#include <iostream>

#include "Process.h"
#include "Time.h"

using namespace std;

void test_time_etc();

/**
 * ich bin main() !
 */
int main() {
    cout << "Hello, World!" << endl << endl;

    cout << "PID " << Std::Process::getpid() << endl;
    cout << "UID " << Std::Process::getuid() << endl;
    cout << "GID " << Std::Process::getgid() << endl;

    test_time_etc();

    Std::Process::exit(121);
    cout << "Bye, World! (and you should not see this BTW)" << endl;
    return 0;
}

void test_time_etc() {
    using Std::Time;
    struct Time::timespec ts = Time::gettimeofday();
    cout << "Seconds      : " << ts.tv.seconds << endl;
    cout << "Microseconds : " << ts.tv.microseconds << endl;
}