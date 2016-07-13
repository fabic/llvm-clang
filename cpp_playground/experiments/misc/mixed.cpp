/** mixed_main.cpp
 *
 * FabiC.2016-06-06
 *
 * Build with:
 *
 * 	   clang++ -Wall -std=c++14 mixed_main.cpp -o mixed_main && ./mixed_main
 */
#include <iostream>
#include <typeinfo>

#include "mixed.hpp"


using std::cout;
using std::endl;


/**
 * MIXED data type prototyping...
 *
 * @link http://www.boost.org/doc/libs/1_61_0/doc/html/variant.html
 * @link http://www.boost.org/doc/libs/1_61_0/libs/convert/doc/html/boost_convert/getting_started.html
 */
void test_fabic_mixed_datatype()
{
    using TLNS::types::mixed;

    cout << "`mixed` typeinfo: " << typeid(mixed).name() << endl;
    std::cout << "sizeof(mixed) : " << sizeof(mixed) << endl;

    auto a = mixed{};

}

void test_typeinfo_etc()
{
    int i = 1024;
    int * pi = &i;
    int& ri = i;
    int*& rpi = pi;

    std::cout << "int is: "   << typeid(int).name()   << endl;
    std::cout << "  i is: "   << typeid(i).name()     << endl;
    std::cout << " pi is: "   << typeid(pi).name()    << endl;
    std::cout << "*pi is: "   << typeid(*pi).name()   << endl;
    std::cout << "ri is: "    << typeid(ri).name()    << endl;
    std::cout << "rpi is: "   << typeid(rpi).name()   << endl;

    std::cout << "sizeof(int) : "       << sizeof(int)      << endl;
    std::cout << "sizeof(unsigned) : "  << sizeof(unsigned) << endl;
    std::cout << "sizeof(long) : "      << sizeof(long) << endl;
    std::cout << "sizeof(long long) : "      << sizeof(long long) << endl;
    std::cout << "sizeof(float) : "     << sizeof(float)    << endl;
    std::cout << "sizeof(double) : "    << sizeof(double)   << endl;

    std::cout << "sizeof(std::string) : "    << sizeof(std::string)   << endl;
}


void test_basic_array_iter()
{
    // Iteration through "raw" arrays :
    int arr[3] = {1,2,3};
    for(auto k : arr) {
        cout << k << ": Hello dude!" << endl;
    }
}

/**
 * MAIN() !
 */
int main(int argc, char *argv[])
{
    test_fabic_mixed_datatype();
    test_typeinfo_etc();
    test_basic_array_iter();

    return 0;
}
