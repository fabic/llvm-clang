// #include <clang/AST/Decl.h>
// #include "clang/AST/DeclCXX.h"
// #include "clang/AST/DeclFriend.h"
// #include "clang/AST/DeclObjC.h"
// #include "clang/AST/DeclOpenMP.h"
#include <clang/AST/Type.h>
#include <llvm/Support/raw_ostream.h>

// clang++ -std=c++1z -Isrc -L/home/fabi/dev/llvm-clang/local/lib -lclang -lLLVMSupport tutu/list-clang-types.cpp -o list-clang-types

int main(int argc, char *argv[])
{
  using namespace clang;

  using llvm::outs;

#define BUILTIN_TYPE(Id, SingletonId)      \
  outs() << " " #Id " "                    \
         << " " #SingletonId " "           \
         << " BuiltinType::Kind::" #Id " " \
         << "\n" ;
#include "clang/AST/BuiltinTypes.def"

  return 0;
}
