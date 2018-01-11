//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

namespace {

/**
 *
 */
class FindNamedClassVisitor
  : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
  /// Ctor
  explicit FindNamedClassVisitor(ASTContext *Context)
    : Context(Context) {}

  /**
   */
  bool VisitRecordDecl(RecordDecl *decl) {
    llvm::errs() << "! [RecordDecl          ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    llvm::errs() << "! [CXXRecordDecl       ] " << Declaration->getQualifiedNameAsString() << "\n";
    if (Declaration->getQualifiedNameAsString() == "n::m::C") {
      FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getLocStart());
      if (FullLocation.isValid())
        llvm::outs() << "Found declaration at "
                     << FullLocation.getSpellingLineNumber() << ":"
                     << FullLocation.getSpellingColumnNumber() << "\n";
    }
    return true;
  }

  /**
   */
  bool VisitEnumDecl(EnumDecl *decl) {
    llvm::errs() << "! [EnumDecl            ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitEnumConstantDecl(EnumConstantDecl *decl) {
    llvm::errs() << "! [EnumConstantDecl    ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitFieldDecl(FieldDecl *decl) {
    llvm::errs() << "! [FieldDecl           ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitFunctionDecl(FunctionDecl *decl) {
    llvm::errs() << "! [FunctionDecl        ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitNamespaceDecl(NamespaceDecl *decl) {
    llvm::errs() << "! [NamespaceDecl       ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitTagDecl(TagDecl *decl) {
    llvm::errs() << "! [TagDecl             ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitTranslationUnitDecl(TranslationUnitDecl *decl) {
    llvm::errs() << "! [TranslationUnitDecl ] ";

    FullSourceLoc FullLocation = Context->getFullLoc(decl->getLocStart());
    // failed
    llvm::errs() << FullLocation.getManager().getFilename( FullLocation )
                 << "\n";
    // failed
    if (FullLocation.isValid())
      llvm::outs() << "  ` Found declaration at "
                   << FullLocation.getSpellingLineNumber() << ":"
                   << FullLocation.getSpellingColumnNumber() << "\n";

    // Trying a manual traversal...

    using clang::DeclContext;
    DeclContext::decl_iterator iter     = decl->decls_begin(),
                               end_iter = decl->decls_end();
    for( /**/ ; iter != end_iter ; iter++) {
      const Decl *D = *iter;
      llvm::errs() << '.';
      if (const NamedDecl *ND = dyn_cast<NamedDecl>(D)) {
        if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
          llvm::errs() << " > Function "
            << (FD->isThisDeclarationADefinition() ? "def.: " : "decl: ")
            << FD->getQualifiedNameAsString()
            << "\n";
        }
        else {
          llvm::errs() << " > [ NamedDecl ]: "
            << ND->getNameAsString()
            << " -- " << ND->getQualifiedNameAsString()
            << "\n";
        }
      }
    }

    llvm::errs() << "\n";

    return true;
  }

  /**
   */
  bool VisitTypeDecl(TypeDecl *decl) {
    llvm::errs() << "! [TypeDecl            ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitTypeAliasDecl(TypeAliasDecl *decl) {
    llvm::errs() << "! [TypeAliasDecl       ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitTypedefDecl(TypedefDecl *decl) {
    llvm::errs() << "! [TypedefDecl         ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitTypedefNameDecl(TypedefNameDecl *decl) {
    llvm::errs() << "! [TypedefNameDecl     ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitValueDecl(ValueDecl *decl) {
    llvm::errs() << "! [ValueDecl           ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitVarDecl(VarDecl *decl) {
    llvm::errs() << "! [VarDecl             ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

  /**
   */
  bool VisitParmVarDecl(ParmVarDecl *decl) {
    llvm::errs() << "! [ParmVarDecl         ] " << decl->getQualifiedNameAsString() << "\n";
    return true;
  }

private:
  ASTContext *Context;
};


/**
 *
 */
class PrintFunctionsConsumer : public ASTConsumer {
  CompilerInstance &Instance;
  std::set<std::string> ParsedTemplates;

public:
  /// Ctor
  PrintFunctionsConsumer(CompilerInstance &Instance,
                         std::set<std::string> ParsedTemplates)
      : Instance(Instance), ParsedTemplates(ParsedTemplates) {}

  /**
   */
  bool HandleTopLevelDecl(DeclGroupRef DG) override {
    for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
      const Decl *D = *i;
      if (const NamedDecl *ND = dyn_cast<NamedDecl>(D)) {
        if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
          llvm::errs() << "Function "
            << (FD->isThisDeclarationADefinition() ? "def.: " : "decl: ")
            << FD->getQualifiedNameAsString()
            << "\n";
        }
        else {
        llvm::errs() << "Top-level-decl: \""
          << ND->getNameAsString()
          << " -- " << ND->getQualifiedNameAsString()
          << "\"\n";
        }
      }
      else {
        llvm::errs() << "Unnamed top-level decl.\"";
      }
    }

    return true;
  }

  /**
   * [HandleTranslationUnit description]
   * @param context [description]
   */
  void HandleTranslationUnit(ASTContext& context) override {
    llvm::errs() << "HandleTranslationUnit() : BEGIN !\n";

    FindNamedClassVisitor Visitor(&context);
    Visitor.TraverseDecl(context.getTranslationUnitDecl());

    llvm::errs() << "HandleTranslationUnit() : END !\n";
  }
};

/**
 *
 */
class PrintFunctionNamesAction : public PluginASTAction {
  std::set<std::string> ParsedTemplates;
protected:
  /**
   */
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return llvm::make_unique<PrintFunctionsConsumer>(CI, ParsedTemplates);
  }

  // Automatically run the plugin after the main AST action
  PluginASTAction::ActionType getActionType() override {
    return AddAfterMainAction;
  }

  /**
   */
  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
      llvm::errs() << "PrintFunctionNames arg = " << args[i] << "\n";

      // Example error handling.
      DiagnosticsEngine &D = CI.getDiagnostics();
      if (args[i] == "-an-error") {
        unsigned DiagID = D.getCustomDiagID(DiagnosticsEngine::Error,
                                            "invalid argument '%0'");
        D.Report(DiagID) << args[i];
        return false;
      } else if (args[i] == "-parse-template") {
        if (i + 1 >= e) {
          D.Report(D.getCustomDiagID(DiagnosticsEngine::Error,
                                     "missing -parse-template argument"));
          return false;
        }
        ++i;
        ParsedTemplates.insert(args[i]);
      }
    }
    if (!args.empty() && args[0] == "help")
      PrintHelp(llvm::errs());

    return true;
  }

  /**
   */
  void PrintHelp(llvm::raw_ostream& ros) {
    ros << "Help for PrintFunctionNames plugin goes here\n";
  }

};

} // global anon. ns.


static FrontendPluginRegistry::Add<PrintFunctionNamesAction>
X("print-fns", "print function names");
