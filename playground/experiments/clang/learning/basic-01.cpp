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

static std::unique_ptr<opt::OptTable> Options( createDriverOptTable() );

//static cl::opt<bool>
//ASTDump("ast-dump", cl::desc(Options->getOptionHelpText(options::OPT_ast_dump)),
//        cl::cat(ClangBasic01Category));

static cl::opt<bool>
  ASTList(
    "ast-list",
    cl::desc( Options->getOptionHelpText( options::OPT_ast_list ) ),
    cl::cat( ClangBasic01Category )
);

//static cl::opt<bool>
//ASTPrint("ast-print",
//         cl::desc(Options->getOptionHelpText(options::OPT_ast_print)),
//         cl::cat(ClangBasic01Category));

//static cl::opt<std::string> ASTDumpFilter(
//    "ast-dump-filter",
//    cl::desc(Options->getOptionHelpText(options::OPT_ast_dump_filter)),
//    cl::cat(ClangBasic01Category));

namespace {

  class ClangCheckActionFactory {
  public:
    std::unique_ptr< clang::ASTConsumer > newASTConsumer()
    {
      if (ASTList)
        return clang::CreateASTDeclNodeLister();

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

  CommonOptionsParser OptionsParser(argc, argv, ClangBasic01Category);

  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  // Clear adjusters because -fsyntax-only is inserted by the default chain.
  Tool.clearArgumentsAdjusters();

  Tool.appendArgumentsAdjuster( getClangStripOutputAdjuster() );

  Tool.appendArgumentsAdjuster(
      getInsertArgumentAdjuster(
          "-fsyntax-only",
          ArgumentInsertPosition::BEGIN)
  );

  ClangCheckActionFactory CheckFactory;

  std::unique_ptr< FrontendActionFactory >
      FrontendFactory = newFrontendActionFactory(&CheckFactory);

  return Tool.run( FrontendFactory.get() );
}
