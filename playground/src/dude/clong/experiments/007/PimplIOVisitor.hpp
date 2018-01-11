#ifndef PIMPLIO_PLUGIN_PimplIOVisitor_HPP
#define PIMPLIO_PLUGIN_PimplIOVisitor_HPP

// #include <clang/AST/RecursiveASTVisitor.h>
// #include <llvm/ADT/MapVector.h>
#include "clong/clang/RecursiveASTVisitor.h"
#include "dude/llvm/MapVector.hpp"
#include "clong/clang/TypePrinter.hpp"
#include "TypeResolver.hpp"
#include "Visitors.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  /**
   *
   */
  class PimplIOVisitor :
          public RecursiveASTVisitor< PimplIOVisitor >
  {
  private:
    using Base = RecursiveASTVisitor< PimplIOVisitor > ;
    using Self = PimplIOVisitor ;


    /**
     *
     */
    struct Artifact {
      const clang::Decl * decl;

      explicit Artifact(const Decl *D) : decl(D) {}
    };

    /**
     *
     */
    struct Repository {
      using Map_t = MapVector< const clang::Decl*, Artifact > ;

      Map_t Artifacts ;

      /// Return the `MapVector(&)` that stores the collected code artifacts.
      inline Map_t& getArtifactsMap() { return Artifacts ; }

      void AddDecl(const Decl *D);
      void AddDeclContext(const DeclContext *DC);
    };

    /**
     *
     */
    class DeclContextStack
            : public llvm::SmallVector< const DeclContext *, /* nElt= */ 32 > {
    public:
      const DeclContext * push( const DeclContext *DC ) {
        push_back( DC );
        return current();
      }

      const DeclContext * pop() {
        auto elt = current();
        pop_back();
        return elt;
      }

      const DeclContext * current() const {
        return back();
      }
    };

  private:
    ASTContext& Context ;
    TypePrinter Printer ;
    Repository  repo ;
    DeclContextStack declContextStack ;

  public:
    /// Ctor
    explicit PimplIOVisitor(ASTContext& Context)
      : Context(Context)
      , Printer(Context.getPrintingPolicy()) {}

    /// Helper for getting the printing policy.
    const PrintingPolicy& getPrintingPolicy() const {
      return Context.getPrintingPolicy();
    }

    /// "Override" - Skip function bodies.
    bool shouldVisitFunctionBodies() const { return false; }

    /**
     * @return The "artifacts" repository.
     */
    inline Repository& getRepository() { return repo; }

    /// "Overriden" so that we may have that `declContextStack` possibility.
    bool TraverseDeclContextHelper(DeclContext *DC);

    bool VisitNamedDecl(NamedDecl *D);
    bool VisitTypeDecl(TypeDecl *D);

    // bool VisitTagDecl(TagDecl *D);
    bool VisitRecordDecl(RecordDecl *decl);
    bool VisitCXXRecordDecl(CXXRecordDecl *D);
    // bool VisitEnumDecl(EnumDecl *decl);
    // bool VisitEnumConstantDecl(EnumConstantDecl *decl);

    // bool VisitFieldDecl(FieldDecl *decl);
    bool VisitDeclaratorDecl(DeclaratorDecl *decl);

    /**
     * Is-a: DeclaratorDecl.
     */
    bool VisitFunctionDecl(FunctionDecl *decl);

    // bool VisitNamespaceDecl(NamespaceDecl *decl);
    // bool VisitTranslationUnitDecl(TranslationUnitDecl *decl);
    // bool VisitTypeAliasDecl(TypeAliasDecl *decl) ;
    // bool VisitTypedefNameDecl(TypedefNameDecl *decl);
    // bool VisitValueDecl(ValueDecl *decl) ;
    // bool VisitVarDecl(VarDecl *decl) ;
    // bool VisitParmVarDecl(ParmVarDecl *decl) ;

    bool TraverseTranslationUnitDecl(TranslationUnitDecl *decl);
    bool TraverseFunctionDecl(FunctionDecl *decl);

  protected:
  };

} // plugin ns
} // clong ns

#endif // PIMPLIO_PLUGIN_PimplIOVisitor_HPP
