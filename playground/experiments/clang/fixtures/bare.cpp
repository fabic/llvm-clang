/** File: bare.cpp
 *
 * FabiC.2014-09-22
 *
 * Build with:
 *
 * 	   clang++ -std=c++11 bare.cpp
 *
 * Dump the AST :
 *
 * 	   clang -Xclang -ast-dump -fsyntax-only -std=c++11 -v bare.cpp
 *
 */

//#include <iostream>
// ^ NOT DOING SO FOR DUMPING AST !

//using std::cout;

int main(int argc, char *argv[])
{
	int arr[3] = {1,2,3};
	int j = 1;

	for(auto i : arr) {
		j += i << 1;
		//cout << i << ": Hello dude!\n";
	}

	return j;
}
