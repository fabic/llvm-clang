/** File: test.cpp
 * FabiC.2014-09-14
 *
 * Build with:
 *
 * 	   clang++ -std=c++11 test.cpp
 */

int main(int argc, char *argv[])
{
	int arr[3] = {1,2,3};
	int j = 127;
	for(int i=3 ; i>0; i--) {
		j -= i;
	}
	return j;
}
