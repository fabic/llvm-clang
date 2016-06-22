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
 *
 * MAIN
 *
 */
int main(int argc, char *argv[])
{
    namespace di = fabic::di;

    auto cnt = di::Container::new_container_instance();

    auto serv1 = std::make_shared<di::service<SomeClassA>>("huh");

    serv1->set_factory_function(
        [](di::base_service::dependencies_map_ref deps) -> std::shared_ptr<SomeClassA> {
            std::cerr << "YEAH! that's service `huh` factory functor bein' invoqued"
                         " which is quite remarkable, actually" << endl;
            return std::make_shared<SomeClassB>(1,2);
        }
    );

    cnt->register_service( serv1 );

    serv1->requires<SomeClassB>("world");
    serv1->requires<SomeClassB>("hello");
    serv1->requires<SomeClassA>("hey");

    auto hey   = std::make_shared<di::service<SomeClassA>>("hey");
    auto hello = std::make_shared<di::service<SomeClassB>>("hello");
    auto world = std::make_shared<di::service<SomeClassB>>("world");

    hey->requires<SomeClassB>("hello");

    hey->set_factory_function(
        [](di::base_service::dependencies_map_ref deps) -> std::shared_ptr<SomeClassA> {
            std::cerr << "YEAH! that's service `hey` factory functor bein' invoqued "
                         "(and I'm a concrete SomeClassB actually)." << endl;
            return std::make_shared<SomeClassB>(1,2);
        }
    );

    hello->set_factory_function(
        [](di::base_service::dependencies_map_ref deps) -> std::shared_ptr<SomeClassB> {
            std::cerr << "YEAH! that's service `hello` factory functor bein' invoqued " << endl;
            return std::make_shared<SomeClassB>(1,2);
        }
    );

    world->set_factory_function(
        [](di::base_service::dependencies_map_ref deps) -> std::shared_ptr<SomeClassB> {
            std::cerr << "YEAH! that's service `hello` factory functor bein' invoqued " << endl;
            return std::make_shared<SomeClassB>(1,2);
        }
    );

    cnt->register_service( hey   );
    cnt->register_service( hello );
    cnt->register_service( world );

    ///////////////////////////////////////////////////////////////////

    auto huh = cnt->get_service<SomeClassA>("huh");

    cout << "Here's type of service `huh` : " << di::type_info(*huh).name() << endl;

    huh->doSomething1();
    cout << "doSomething: ok" << endl;
    cout << "getThing: " << huh->getThing() << endl;


    //cnt->loadFromYamlFile("test_container.yml");


    cnt->debugDumpContainer(std::cout);

    return 0;
}
