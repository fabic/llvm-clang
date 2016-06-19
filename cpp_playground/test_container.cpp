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
//#include "SomeClassA.hpp"
#include "SomeClassB.hpp"


using std::cout;
using std::endl;


int main(int argc, char *argv[])
{
    namespace di = fabic::di;

    di::Container cnt;

    auto b = new SomeClassB(2, 4);

    //cnt.registerService("huh", b, typeid(b).name());
    cnt.registerService<SomeClassA>("huh", *b);

    auto& a = cnt.get_service<SomeClassA>("huh");
    cout << "Here's type of service `huh` : " << di::type_info(a).name() << endl;

    auto def = cnt.new_service_definition<SomeClassA>("huh");

    def.requires<SomeClassB>("hah");
    def.requires<SomeClassB>("heeeey");

    cnt.new_service_definition<SomeClassB>("hey.b.1");
    cnt.new_service_definition<SomeClassB>("hey.b.2");

    //SomeClassA& aaa = cnt.get_service("huh");

//    if (p) {
//    	//SomeClassA * b = (SomeClassA *) p;
//    	SomeClassA * b = static_cast<SomeClassA *>( p );
//    	int z = b->getThing();
//        std::cout << "Z: " << z << std::endl;
//    }

    //

    //cnt.loadFromYamlFile("test_container.yml");

    cnt.debugDumpContainer(std::cout);

    return 0;
}
