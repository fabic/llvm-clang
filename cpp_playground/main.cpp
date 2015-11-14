#include <iostream>

#include "Process.h"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    Std::Process::exit(121);
    cout << "Bye, World!" << endl;
    return 0;
}