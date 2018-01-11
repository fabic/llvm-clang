#include <clang/AST/Decl.h>
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclFriend.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/DeclOpenMP.h"
#include <llvm/Support/raw_ostream.h>

// clang++ -std=c++1z -Isrc -L/home/fabi/dev/llvm-clang/local/lib -lclang -lLLVMSupport tutu/list-clang-typelocs.cpp -o list-clang-typelocs

int main(int argc, char *argv[])
{
  using namespace clang;

  using llvm::outs;

  // outs() << "LIST OF Clang's TypeLoc classes :\n";

#define ABSTRACT_TYPELOC(CLASS, PARENT)
#define TYPELOC(CLASS, PARENT)     \
  outs() << " " #CLASS " : "       \
         << " " #PARENT " : "      \
         << sizeof(CLASS##TypeLoc) \
         << " : "                  \
         << sizeof(PARENT)         \
         << "\n" ;
#include "clang/AST/TypeLocNodes.def"

  return 0;
}
