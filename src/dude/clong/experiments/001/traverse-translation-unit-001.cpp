//===- traverse-translation-unit-001.cpp ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// FABIC / 2017-12-17
//
// Experiment -- traversing a translation unit _without_resorting to
// `RecursiveASTVisitor<...>`.
//
// Usage :
//   $ clang++ -Xclang -load -Xclang build/translation-unit/001/traverse-translation-unit-001.so -std=c++1z tutu/test-003.cpp
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

namespace clong {

  /**
   *
   */
  class TraverseTUnitConsumer : public ASTConsumer {
  private:
    CompilerInstance &Instance;
    std::set<std::string> ParsedTemplates;

  public:
    /// Ctor
    TraverseTUnitConsumer(CompilerInstance &Instance,
                           std::set<std::string> ParsedTemplates)
        : Instance(Instance), ParsedTemplates(ParsedTemplates) {}

    /**
     */
    void HandleTranslationUnit(ASTContext& context) override;

  private:
    /**
     * Dispatches Decl node processing to the VisitXXX() methods.
     *
     * @param D Any concrete Decl sub-type.
     */
    void VisitDecl(const Decl *D);

    /**
     * Iterate over the sub-nodes of the decl. context `DC`, invoking
     * `VisitDecl()`.
     */
    void VisitDeclContext(const DeclContext *DC);

    /**
     */
    void VisitTranslationUnit(const TranslationUnitDecl *ND);

    /**
     */
    void VisitNamespace(const NamespaceDecl *ND);

    /**
     */
    void VisitFunction(const FunctionDecl *FD);

  private:
    void VisitVarDecl(const VarDecl *VD);
  };

  // [overriden, entry point]
  void TraverseTUnitConsumer::HandleTranslationUnit(ASTContext& context)
  {
    llvm::errs() << "-- HandleTranslationUnit() : BEGIN ! --\n";

    clang::TranslationUnitDecl *TU = context.getTranslationUnitDecl();

    VisitDecl( TU );

    llvm::errs() << "-- HandleTranslationUnit() : END ! --\n";
  }

  void TraverseTUnitConsumer::VisitDecl(const Decl *D)
  {
      // is-a VarDecl
      if (const ParmVarDecl *PV = dyn_cast<ParmVarDecl>(D)) {
        llvm::errs() << "    ` [ParmVarDecl] (!!): "
          << PV->getQualifiedNameAsString() << "\n";;
        VisitVarDecl( PV );
      }
      // is-a VarDecl
      else if (const ImplicitParamDecl *IP = dyn_cast<ImplicitParamDecl>(D)) {
        llvm::errs() << " > [ ImplicitParamDecl ] " << IP->getQualifiedNameAsString() << "\n";
      }
      // is-a DeclaratorDecl & Redeclarable<VarDecl>
      else if (const VarDecl *VD = dyn_cast<VarDecl>(D)) {
        llvm::errs() << " > [ VarDecl ] " << VD->getQualifiedNameAsString() << " -- "
          << "\n";
      }
      else if (const FileScopeAsmDecl *VD = dyn_cast<FileScopeAsmDecl>(D)) {
        llvm::errs() << " > [ FileScopeAsmDecl ] ( !!! )" << "\n";
      }
      /*
         DeclContext concretes.
       */
      else if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
        VisitFunction( FD );
      }
      else if (const CXXRecordDecl *RD = dyn_cast<CXXRecordDecl>(D)) {
        llvm::errs() << " > [ CXXRecordDecl ] " << RD->getQualifiedNameAsString() << "\n";
        VisitDeclContext( RD );
      }
      else if (const RecordDecl *RD = dyn_cast<RecordDecl>(D)) {
        llvm::errs() << " > [ RecordDecl ] " << RD->getQualifiedNameAsString() << "\n";
        VisitDeclContext( RD );
      }
      else if (const EnumDecl *ED = dyn_cast<EnumDecl>(D)) {
        llvm::errs() << " > [ EnumDecl ]\n";
        VisitDeclContext( ED );
      }
      // enum/struct/union/class (we should never enter this for we should
      // have matched an earlier case.
      else if (const TagDecl *TD = dyn_cast<TagDecl>(D)) {
        llvm::errs() << " > [ TagDecl ] (??)\n";
        VisitDeclContext( TD );
      }
      else if (const NamespaceDecl *ND = dyn_cast<NamespaceDecl>(D)) {
        VisitNamespace( ND );
      }
      else if (const TranslationUnitDecl *TU = dyn_cast<TranslationUnitDecl>(D)) {
        VisitTranslationUnit( TU );
      }
      else if (const BlockDecl *BD = dyn_cast<BlockDecl>(D)) {
        llvm::errs() << " > [ BlockDecl ]" << "\n";
        VisitDeclContext( BD );
      }
      else if (const ExternCContextDecl *EC = dyn_cast<ExternCContextDecl>(D)) {
        llvm::errs() << " > [ ExternCContextDecl ]" << "\n";
        VisitDeclContext( EC );
      }
      else if (const ExportDecl *ED = dyn_cast<ExportDecl>(D)) {
        llvm::errs() << " > [ ExportDecl ]" << "\n";
        VisitDeclContext( ED );
      }
      else if (const DeclContext *DC = dyn_cast<DeclContext>(D)) {
        llvm::errs() << " > [ DeclContext ] (!!!) " << "\n";
        VisitDeclContext( DC );
      }
      else if (const TypedefNameDecl *TD = dyn_cast<TypedefNameDecl>(D)) {
        llvm::errs() << " > [ TypedefNameDecl ]\n";
      }
      else if (const TypedefDecl *TD = dyn_cast<TypedefDecl>(D)) {
        llvm::errs() << " > [ TypedefDecl ]\n";
      }
      else if (const TypeAliasDecl *TD = dyn_cast<TypeAliasDecl>(D)) {
        llvm::errs() << " > [ TypeAliasDecl ]\n";
      }
      else if (const TypeDecl *TD = dyn_cast<TypeDecl>(D)) {
        llvm::errs() << " > [ TypeDecl ]\n";
      }
      else if (const LabelDecl *LD = dyn_cast<LabelDecl>(D)) {
        llvm::errs() << " > [ LabelDecl ]: " << LD->getQualifiedNameAsString() << "\n";
      }
      else if (const EnumConstantDecl *EC = dyn_cast<EnumConstantDecl>(D)) {
        llvm::errs() << " > [ EnumConstantDecl ]: " << EC->getQualifiedNameAsString() << "\n";
      }
      else if (const IndirectFieldDecl *FD = dyn_cast<IndirectFieldDecl>(D)) {
        llvm::errs() << " > [ IndirectFieldDecl ] -_-" << "\n";
      }
      // is-a DeclaratorDecl & Mergeable<FieldDecl>
      else if (const FieldDecl *FD = dyn_cast<FieldDecl>(D)) {
        llvm::errs() << " > [ FieldDecl ]: " << FD->getQualifiedNameAsString() << "\n";
      }
      // is-a ValueDecl
      else if (const DeclaratorDecl *DD = dyn_cast<DeclaratorDecl>(D)) {
        llvm::errs() << " > [ DeclaratorDecl ]: " << DD->getQualifiedNameAsString() << "\n";
      }
      // is-a NamedDecl
      else if (const ValueDecl *VD = dyn_cast<ValueDecl>(D)) {
        llvm::errs() << " > [ ValueDecl ]: " << VD->getQualifiedNameAsString() << "\n";
      }
      else if (const NamedDecl *ND = dyn_cast<NamedDecl>(D)) {
        llvm::errs() << " > [ NamedDecl ]: "
          << ND->getNameAsString()
          << " -- " << ND->getQualifiedNameAsString()
          << "\n";
      }
      else if (const EmptyDecl *LD = dyn_cast<EmptyDecl>(D)) {
        llvm::errs() << " > [ EmptyDecl ]: " << "\n";
      }
      else {
        llvm::errs() << " > [ unknown-decl or unnamed-decl (?) ]\n";
      }
  }

  void TraverseTUnitConsumer::VisitDeclContext(const DeclContext *DC)
  {
    clang::DeclContext::decl_iterator iter     = DC->decls_begin(),
                                      end_iter = DC->decls_end();

    for( /**/ ; iter != end_iter ; iter++)
    {
      const Decl *D = *iter;

      llvm::errs() << '.';

      VisitDecl( D );
    }
  }

  void TraverseTUnitConsumer::VisitTranslationUnit(const TranslationUnitDecl *ND)
  {
    llvm::errs() << "TranslationUnit (...)" << "\n";
    VisitDeclContext( ND );
  }

  void TraverseTUnitConsumer::VisitNamespace(const NamespaceDecl *ND)
  {
    llvm::errs() << " > Namespace "<< ND->getQualifiedNameAsString() << "\n";
    VisitDeclContext( ND );
  }

  void TraverseTUnitConsumer::VisitFunction(const FunctionDecl *FD)
  {
    llvm::errs() << " > Function "
      << (FD->isThisDeclarationADefinition() ? "def.: " : "decl: ")
      << FD->getQualifiedNameAsString() << "\n";

    // Iterate over this function's arguments.
    auto it     = FD->param_begin();
    auto it_end = FD->param_end();

    for( ; it != it_end; it++) {
      const ParmVarDecl *P = *it;
      llvm::errs() << "    ` (*) " << P->getQualifiedNameAsString() << "\n";
    }

    // Walk the re-declaration chain.
    const FunctionDecl *decl = FD;
    while(nullptr != (decl = decl->getPreviousDecl())) {
      llvm::errs() << "    ` (!) RE-DECL. (!)";
      VisitFunction( decl );
    }

    VisitDeclContext( FD );

#if 0
    // Function _definitions_ have their arguments disposed as sub-nodes,
    // this isn't the case of function declarations.
    clang::DeclContext::decl_iterator iter     = FD->decls_begin(),
                                      end_iter = FD->decls_end();
    for( /**/ ; iter != end_iter ; iter++)
    {
      const Decl *D = *iter;

      if (const ParmVarDecl *PV = dyn_cast<ParmVarDecl>(D)) {
        llvm::errs() << "    ` ParmVarDecl: "
                     << PV->getQualifiedNameAsString()
                     << "\n";;
      }
    }
#endif // 0

    llvm::errs() << "    ` Done with function.\n";
  }

    void TraverseTUnitConsumer::VisitVarDecl(const VarDecl *VD)
    {
      using clang::QualType;
      using clang::Type;
      // Beware: that thing is some sort of smart-pointer.
      // This is actually a Type*
      QualType qtype = VD->getTypeSourceInfo()->getType();
      Type::TypeClass tc = qtype->getTypeClass();

      using TC = clang::Type::TypeClass;
      switch( tc ) {
        case TC::Builtin:
          llvm::errs() << "        ` [builtin]\n";
          break;
        default:
          llvm::errs() << "(!!!) unknown type class\n";
      }
    }

  /**
   *
   */
  class TraverseTUnitAction : public PluginASTAction {
    std::set<std::string> ParsedTemplates;
  protected:
    /**
     */
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   llvm::StringRef) override {
      return llvm::make_unique<TraverseTUnitConsumer>(CI, ParsedTemplates);
    }

    // Automatically run the plugin after the main AST action
    PluginASTAction::ActionType getActionType() override {
      return AddAfterMainAction;
    }

    /**
     */
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override
    {
      if (!args.empty() && args[0] == "help")
        PrintHelp(llvm::errs());

      for (unsigned i = 0, e = args.size(); i != e; ++i) {
        llvm::errs() << "TraverseTranslationUnit arg = " << args[i] << "\n";
      }

      return true;
    }

    /**
     */
    void PrintHelp(llvm::raw_ostream& ros) {
      ros << "Help for TraverseTranslationUnit plugin goes here\n";
    }

  };

} // clong ns.


static FrontendPluginRegistry::Add<clong::TraverseTUnitAction>
  X("print-fns", "print function names");
