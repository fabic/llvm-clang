/** File: test.cpp
 * FabiC.2014-09-14
 *
 * Build with:
 *
 * 	   clang++ -std=c++11 test.cpp
 */

#include <iostream>

using std::cout;

int main(int argc, char *argv[])
{
	int arr[3] = {1,2,3};
	for(auto i : arr)
		cout << i << ": Hello dude!\n";
	return 127;
}
