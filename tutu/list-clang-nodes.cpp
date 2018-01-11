#include <iostream>
#include <clang/AST/Decl.h>
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclFriend.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/DeclOpenMP.h"

// clang++ -std=c++1z tutu/list-clang-nodes.cpp -lclang -lLLVM-5.0
//
// clang++ -std=c++1z tutu/list-clang-nodes.cpp -L/home/fabi/dev/llvm-clang/local/lib -lclang -lLLVMSupport

int main(int argc, char *argv[])
{
  using namespace std;
  using namespace clang;

#define DECL(CLASS, BASE) \
  cout << " > " #CLASS " : " << "sizeof(" #CLASS "Decl) : " << sizeof(CLASS##Decl) << " : " \
    << " : " #BASE " : "   \
    << sizeof(CLASS##Decl) - sizeof(BASE) \
    << endl;
#include "clang/AST/DeclNodes.inc"

  return 0;
}
