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


/**
 * MAIN
 */
int main(int argc, char *argv[])
{
    namespace di = fabic::di;

    di::Container cnt;

    auto b = new SomeClassB(2, 4);

    //cnt.registerService("huh", b, typeid(b).name());
//    cnt.registerService<SomeClassA>("huh", *b);

//    auto a = cnt.get_service<SomeClassA>("huh");
//    cout << "Here's type of service `huh` : " << di::type_info(a).name() << endl;

    auto def = cnt.new_service_definition<SomeClassA>("huh");

    def.requires<SomeClassB>("huh_first_dep");
    def.requires<SomeClassB>("huh_second_dep");

    def.set_factory_function(
        [](di::base_service_definition::dependencies_map& deps) -> std::shared_ptr<SomeClassA> {
            std::cerr << "YEAH! that's service `huh` factory functor bein' invoqued"
                         " which is quite remarkable, actually" << endl;
            return std::make_shared<SomeClassB>(1,2);
        }
    );

    //auto ttt = def.get_instance();

    auto huh = cnt.get_service<SomeClassA>("huh");

    cout << "Here's type of service `huh` : " << di::type_info(*huh).name() << endl;

    huh->doSomething1();
    cout << "doSomething: ok" << endl;
    cout << "getThing: " << huh->getThing() << endl;

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
