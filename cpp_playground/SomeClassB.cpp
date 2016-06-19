#include "SomeClassB.hpp"

SomeClassB::SomeClassB(int _x, int _y, int _z)
        : x(_x), y(_y), z(_z) {
}

void SomeClassB::doSomethingElse() {
    x += std::rand();
    y += std::rand();
    z += x + y;
}

int SomeClassB::getThing() {
    return z;
}

//SomeClassB *SomeClassB::instantiateService(fabic::di::ServiceDefinition &def) {
//    return nullptr;
//}
