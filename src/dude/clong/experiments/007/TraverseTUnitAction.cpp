
#include <clang/Frontend/FrontendPluginRegistry.h>
#include "TraverseTUnitAction.hpp"
#include "TraverseTUnitConsumer.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  // ctor
  TraverseTUnitAction::TraverseTUnitAction()
  {
    (terrs().yellow() << "TraverseTUnitAction() [ctor]\n").reset()
      .indent();
  }

  // dtor
  TraverseTUnitAction::~TraverseTUnitAction()
  {
    terrs(-1).unindent();
    (terrs().yellow() << "~TraverseTUnitAction() [dtor]\n").reset();
  }


  std::unique_ptr<ASTConsumer>
    TraverseTUnitAction::CreateASTConsumer(CompilerInstance &CI,
                                           llvm::StringRef InFile)
    {
      (terrs().green().bold() << "TraverseTUnitAction::CreateASTConsumer(): "
        << "Infile: " << InFile << "\n")
        .reset();
      return llvm::make_unique< TraverseTUnitConsumer >(CI, ParsedTemplates);
    }


  PluginASTAction::ActionType
    TraverseTUnitAction::getActionType() {
      // return AddAfterMainAction;
      return AddBeforeMainAction;
    }


  bool
    TraverseTUnitAction::ParseArgs(const CompilerInstance &CI,
                                   const std::vector<std::string> &args)
    {
      (terrs().green().bold() << "TraverseTUnitAction::ParseArgs()\n")
        .reset();

      if (!args.empty() && args[0] == "help")
        PrintHelp(llvm::errs());

      for (unsigned i = 0, e = args.size(); i != e; ++i) {
        llvm::errs() << "TraverseTranslationUnit arg = " << args[i] << "\n";
      }

      return true;
    }


  void
    TraverseTUnitAction::PrintHelp(llvm::raw_ostream& ros) {
      ros << "Help for TraverseTranslationUnit plugin goes here\n";
    }

  bool
    TraverseTUnitAction::BeginInvocation(CompilerInstance &CI)
    {
      (terrs().green() << "TraverseTUnitAction::BeginInvocation()\n")
        .reset();
      return true;
    }

  bool
    TraverseTUnitAction::BeginSourceFileAction(CompilerInstance &CI)
    {
      (terrs().green() << "TraverseTUnitAction::BeginSourceFileAction()\n")
        .reset();
      return true;
    }

  void
    TraverseTUnitAction::ExecuteAction()
    {
      (terrs().green() << "TraverseTUnitAction::ExecuteAction()\n")
        .reset();
      Base::ExecuteAction();
    }

  void
    TraverseTUnitAction::EndSourceFileAction()
    {
      (terrs().green() << "TraverseTUnitAction::EndSourceFileAction()\n")
        .reset();
    }

} // plugin ns.
} // clong ns.


// ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

/**
 * Clang plugin registration
 *
 * * `clang/Frontend/FrontendPluginRegistry.h`
 */
static
  clang::FrontendPluginRegistry::Add< clong::plugin::TraverseTUnitAction >
    X("print-fns", "print function names");

// ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
