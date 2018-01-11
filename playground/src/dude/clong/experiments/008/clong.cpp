//===- traverse-translation-unit-001.cpp ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// FABIC / 2017-12-21
//
// I've got a better understanding of that AST and how to traverse it manually.
// Let's investigate Clang's `RecursiveASTVisitor< Derived >` with a new look.
//
// Done here: importing Clang's TypePrinter.cpp file, which does not have a
//            corresponding .h header --> so as to print out all the Type/s
//            the ASTContext knows about.
//            See method `TraverseTUnitConsumer::InvestigateASTContextTypes()`
//
//            And it does work ^^
//
// Usage :
//
//   $ clang++ -Xclang -load -Xclang build/translation-unit/005/clong-05.so -std=c++1z tutu/test-004.cpp
//
//===----------------------------------------------------------------------===//

// ?? #include "llvm/Support/raw_ostream.h"
// ?? #include "clang/Sema/Sema.h"
#include "dude/util/Terminal.hpp"

//using namespace clang;

namespace clong {  } // clong ns.

using namespace clong;

extern "C"
void _clong_so_init() {
  /* Doing anything here is questionable... */
}

extern "C"
void _clong_so_fini() {
  /* Doing anything here is quite a bad idea. */
}
