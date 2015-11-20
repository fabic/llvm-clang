#include <iostream>


class SomeClassA {
	int a, b, c, d;
private:
	SomeClassA();
public:
	//SomeClassA() : a(1), b(2), c(3) {}
	SomeClassA(bool b, int i = 1024, long l = 2048, float f = 1.23f, double d = 4.578, int *pi = 0, long *pl = 0);
	// SomeClassA(bool b = true, int i = 1024, long l = 2048, float f = 1.23f, double d = 4.578, int *pi = 0, long *pl = 0) {}

	virtual ~SomeClassA() {}

	void doSomething1() {
		std::srand(c);
		std::srand(d);
		a += std::rand();
		b += std::rand();
	}

	int getThing() { return d + std::rand(); }
};
