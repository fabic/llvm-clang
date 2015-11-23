#include <cstdlib>

#include "ServiceDefinition.hpp"

class SomeClassB {
	int x, y, z;
private:
	SomeClassB() {}
public:
	SomeClassB(int _x, int _y, int _z = 128);

	virtual ~SomeClassB() {}

	void doSomethingElse();

	int getThing();

    static SomeClassB * instantiateService(fabic::di::ServiceDefinition& def);
};
