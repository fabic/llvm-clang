#include <cstdlib>

class SomeClassA {
	int a, b, c, d;
protected:
	SomeClassA() {}
public:
	//SomeClassA() : a(1), b(2), c(3) {}
	SomeClassA(bool b, int i = 11, long l = 2048, float f = 1.23f, double d = 4.578,
		int *pi = 0, long *pl = 0, int r = 12, int s = 13, int t = 14, int u = 15,
		int v = 16, int w = 17, char x = 'a');
	// SomeClassA(bool b = true, int i = 1024, long l = 2048, float f = 1.23f, double d = 4.578, int *pi = 0, long *pl = 0) {}

	virtual ~SomeClassA() {}

	void doSomething1();

	int getThing() { return d + std::rand(); }
};
