/** test2.cpp
 *
 * FabiC.2014-09-14
 *
 * Build with:
 *
 * 	   clang++ -std=c++11 test2.cpp SomeClassA.cpp -o test2
 */
#include <iostream>
#include <typeinfo>

#include "SomeClassA.hpp"

using std::cout;
using std::endl;


int main(int argc, char *argv[])
{
	int arr[3] = {1,2,3};
	for(auto k : arr)
		; //cout << k << ": Hello dude!" << endl;

	const std::type_info& ta = typeid(SomeClassA);

	int i = 1024;
	int * pi = 0;

	auto cls_a = new SomeClassA(true);

/*
	std::cout << "int is: "   << typeid(int).name()   << '\n';
	std::cout << "  i is: "   << typeid(i).name()     << '\n';
	std::cout << " pi is: "   << typeid(pi).name()    << '\n';
	std::cout << "*pi is: "   << typeid(*pi).name()   << '\n';

	std::cout << "cls_a is: " << typeid(cls_a).name() << '\n';
	std::cout << "sizeof(int) : " << sizeof(int) << '\n';
	std::cout << "sizeof(SomeClassA) : " << sizeof(SomeClassA) << '\n';
	std::cout << "SomeClassA is: "  << typeid(SomeClassA).name() << '\n';
*/
	auto cls_a2 = new SomeClassA(true);

	cls_a2->doSomething1();

	auto z = cls_a2->getThing();

	auto x = z + std::rand();
//	cout << z;

	return x + z;
}
