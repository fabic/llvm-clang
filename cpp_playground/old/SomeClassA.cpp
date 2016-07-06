#include "SomeClassA.hpp"

//extern
SomeClassA::SomeClassA(bool boo, int i, long l, float f, double d,
	int *pi, long *pl, int r, int s, int t, int u,
	int v, int w, char x ) : c(3) {
	std::srand(c);
	a = std::rand() + c;
	b = std::rand() + boo;
	d = a+b+c;
}

void SomeClassA::doSomething1() {
	std::srand(c);
	std::srand(d);
	a += std::rand();
	b += std::rand();
}
