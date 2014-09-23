/** File: bare2.cpp
 * FabiC.2014-09-22
 *
 * Build with:
 *
 * 	   clang++ -std=c++11 bare.cpp bare2.cpp
 *
 * Dump the AST :
 *
 * 	   clang -Xclang -ast-dump -fsyntax-only -std=c++11 -v bare.cpp bare2.cpp
 *
 */

//#include <iostream>
// ^ NOT DOING SO FOR DUMPING AST !

//using std::cout;

int dummy_func1 (SomeType arg1, int arg2, int *list)
{
	HeyType a, b;

	int i = a();
	int j = b();
	
	ByeType c[10];

	//int l = 0;

	for(auto k : arr) {
		l += k<<1;
	}

	a.method1(1,2,3,4);
	b.thing("hello world");

	auto w = a + b;

	return l;
}
