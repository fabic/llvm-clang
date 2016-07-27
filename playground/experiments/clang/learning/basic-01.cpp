//===--- tools/clang-check/ClangCheck.cpp - Clang check tool --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file implements a clang-check tool that runs clang based on the info
//  stored in a compilation database.
//
//  This tool uses the Clang Tooling infrastructure, see
//    http://clang.llvm.org/docs/HowToSetupToolingForLLVM.html
//  for details on setting it up with LLVM source tree.
//
//===----------------------------------------------------------------------===//
// FabiC/2016-01-03 : Imported here from Clang's
//   `llvm-clang/clang/tools/clang-check/ClangCheck.cpp`
//===----------------------------------------------------------------------===//

#include "clang/AST/ASTConsumer.h"
#include "clang/CodeGen/ObjectFilePCHContainerOperations.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Rewrite/Frontend/FrontendActions.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"

#include <iostream>

# include "clong/ClangAstConsumer.hpp"

using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp(
    "\tFor example, to run clang-check on all files in a subtree of the\n"
    "\tsource tree, use:\n"
    "\n"
    "\t  find path/in/subtree -name '*.cpp'|xargs clang-check\n"
    "\n"
    "\tor using a specific build path:\n"
    "\n"
    "\t  find path/in/subtree -name '*.cpp'|xargs clang-check -p build/path\n"
    "\n"
    "\tNote, that path/in/subtree and current directory should follow the\n"
    "\trules described above.\n"
    "\n"
);

static cl::OptionCategory ClangBasic01Category("clang-basic-01 options");

static std::unique_ptr< opt::OptTable > Options( createDriverOptTable() );

static cl::opt<bool>
  OptHelloWorld(
    "hello-world",
    cl::desc("Hello world !"),
    cl::cat( ClangBasic01Category )
);

static cl::opt<bool>
  ASTList(
    "ast-list",
    cl::desc( Options->getOptionHelpText( options::OPT_ast_list ) ),
    cl::cat( ClangBasic01Category )
);

//static cl::opt<std::string> ASTDumpFilter(
//    "ast-dump-filter",
//    cl::desc(Options->getOptionHelpText(options::OPT_ast_dump_filter)),
//    cl::cat(ClangBasic01Category));

namespace {

  using namespace TLNS::clong;

  class HelloWorldActionFactory {
  public:
    std::unique_ptr< clang::ASTConsumer > newASTConsumer()
    {
      if (ASTList)
        return clang::CreateASTDeclNodeLister();
      else if (OptHelloWorld)
        return ClangASTConsumer::create();

      return llvm::make_unique< clang::ASTConsumer >();
    }
  };

} // namespace


/**
 * MAIN() !
 */
int main(int argc, const char **argv)
{
  llvm::sys::PrintStackTraceOnErrorSignal();

  // Initialize targets for clang module support.
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmPrinters();
  llvm::InitializeAllAsmParsers();

  CommonOptionsParser optionsParser(argc, argv, ClangBasic01Category);

#if 1
    std::cout << "# Compilation database sources :" << std::endl;

    for (auto &fileName : optionsParser.getCompilations().getAllFiles()) {
      std::cout << "  * " << fileName << std::endl;
    }

    std::cout << "# Sources (command line) :" << std::endl;

    for (auto &fileName : optionsParser.getSourcePathList()) {
      std::cout << "# " << fileName << std::endl;
    }
#endif // 1

  ClangTool Tool(optionsParser.getCompilations(),
                 optionsParser.getCompilations().getAllFiles());

  // Clear adjusters because -fsyntax-only is inserted by the default chain.
  Tool.clearArgumentsAdjusters();

  Tool.appendArgumentsAdjuster( getClangStripOutputAdjuster() );

  Tool.appendArgumentsAdjuster(
      getClangSyntaxOnlyAdjuster()
      // getInsertArgumentAdjuster(
      //     "-fsyntax-only",
      //     ArgumentInsertPosition::BEGIN
      // )
  );

  HelloWorldActionFactory CheckFactory;

  std::unique_ptr< FrontendActionFactory >
      FrontendFactory = newFrontendActionFactory(&CheckFactory);

  return Tool.run( FrontendFactory.get() );
}
