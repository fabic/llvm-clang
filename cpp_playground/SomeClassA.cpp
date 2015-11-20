#include "SomeClassA.hpp"

//extern
SomeClassA::SomeClassA(bool boo, int i, long l, float f, double d, int *pi, long *pl) : c(3) {
	std::srand(c);
	a = std::rand() + c;
	b = std::rand() + boo;
	d = a+b+c;
}
