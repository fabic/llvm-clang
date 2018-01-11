#include <iostream>
#include <clang/AST/Decl.h>

// clang++ -I../local/include/ -Isrc -std=c++1z -stdlib=libc++ tutu/clang-version.cpp -o clang-version -L ../local/lib -lLLVMSupport

int main(int argc, char *argv[])
{
  using namespace std;
  using namespace clang;

  cout << "Version : " __VERSION__ << endl;

  return 0;
}
