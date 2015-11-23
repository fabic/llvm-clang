/** test2.cpp
 *
 * FabiC.2014-09-14
 *
 * Build with:
 *
 * 	   clang++ -std=c++11 test_container.cpp container.cpp SomeClassA.cpp -o test_container
 */
#include <iostream>

#include "container.hpp"
#include "SomeClassA.hpp"
#include "SomeClassB.hpp"


using std::cout;
using std::endl;


int main(int argc, char *argv[])
{
    using fabic::di::Container;

    Container cnt;

    auto a = new SomeClassA(true);

    cnt.registerService("huh", a, typeid(a).name());

    void *p = cnt.get_service("huh");

    if (p) {
    	//SomeClassA * b = (SomeClassA *) p;
    	SomeClassA * b = static_cast<SomeClassA *>( p );
    	int z = b->getThing();
        std::cout << "Z: " << z << std::endl;
    }

    //

    cnt.loadFromYamlFile("test_container.yml");

    cnt.debugDumpContainer(std::cout);

    return 0;
}
