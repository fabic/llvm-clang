#ifndef PIMPLIO_PLUGIN_PimplIOVisitor_HPP
#define PIMPLIO_PLUGIN_PimplIOVisitor_HPP

#include <memory>
// #include <clang/AST/RecursiveASTVisitor.h>
// #include <llvm/ADT/MapVector.h>
#include "dude/clang/RecursiveASTVisitor.h"
#include "dude/clang/TypePrinter.hpp"
#include "dude/llvm/MapVector.hpp"
#include "TypeResolver.hpp"
//#include "Visitors.hpp"
#include "visitors/XBase.hpp"
#include "visitors/XDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;


  /**
   *
   */
  // class Artifact {
  // private:
  //   std::unique< XBase > Elt_;
  //
  //
  //   explicit Artifact(std::unique<XBase> Elt) : decl(D) {}
  // };


  /**
   *
   */
  class Repository {
  public:
    /// Key type may be a `const Decl*` or `const Type*`.
    using Key_t     = size_t;
    using Pointer_t = std::shared_ptr< XBase > ;
    using Map_t     = MapVector< Key_t, Pointer_t > ;

  public:
    static Key_t KeyOf(std::shared_ptr<XDecl>& D) {
      return reinterpret_cast<size_t>( D->getDeclPtr() );
    }

  private:
    Map_t Artifacts ;

  public:
    /// Return the `MapVector(&)` that stores the collected code artifacts.
    inline Map_t& getArtifactsMap() { return Artifacts ; }

    // void AddDecl(const Decl *D);
    // void AddDeclContext(const DeclContext *DC);
    void Add(std::shared_ptr<XDecl> D);
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


  /**
   * Implementation is scattered into `PimplIOVisitor.cpp` _and_ into
   * the various `visitors/XxxxDeclVisitor.cpp` impl. files.
   *
   * Following this code is not (that much) straightforward due to the fact
   * that we're resorting to Clang's RecursiveASTVisitor<>, which is cool since
   * we just have to let it drive us through the arcanes of ASTs; however it
   * has the drawback that our implementation is a bit less straightforward
   * since we're handed over control in an "event-fashion" (through the
   * Traverse/WalkUpFrom/Visit-XxxxDecl() methods).  Hence it is and feels
   * unatural to keep track of context since we can't pass additional arguments
   * to methods.
   *
   */
  class PimplIOVisitor :
          public RecursiveASTVisitor< PimplIOVisitor >
  {
  private:
    using Base = RecursiveASTVisitor< PimplIOVisitor > ;
    using Self = PimplIOVisitor ;

  private:
    ASTContext& Context ;
    TypePrinter Printer ;
    Repository  repo ;
    DeclContextStack declContextStack ;

  public:
    /// Ctor
    explicit PimplIOVisitor(ASTContext& Context);
    ~PimplIOVisitor();

    /// The ASTContext.
    const ASTContext& getContext() const { return Context; }

    /// The `TypePrinter` (code was borrowed from Clang btw.)
    TypePrinter& getTypePrinter() { return Printer; }

    /// Helper for getting the printing policy.
    const PrintingPolicy& getPrintingPolicy() const {
      return Context.getPrintingPolicy();
    }

    /// "Override" - Skip function bodies.
    bool shouldVisitFunctionBodies() const { return false; }

    /// Helper for getting the FileEntry from whence this declaration is.
    const FileEntry* getFileEntry(const Decl& D) const;

    /// Helper for getting a file name for whence the give decl. comes from.
    StringRef getFileName(const Decl& D) const {
      const FileEntry* FE = getFileEntry( D );
      return FE != nullptr ? FE->getName() : "<< Decl has no file. >>";
    }

    /// Helper that checks if the given declaration is from the "main file".
    bool isInMainFile(const Decl& D) const;

    /**
     * @return The "artifacts" repository.
     */
    inline Repository& getRepository() { return repo; }

    /// Helper for instantiating XxxxDecl "visitors" (managed by means of
    /// `shared_ptr<>`) and have these managed through the "repository".
    template<typename X, typename T>
      std::shared_ptr<X>
        CreateX(const T& Decl) {
          auto S = std::make_shared<X>(*this, Decl);
          getRepository().Add( std::static_pointer_cast<XDecl>(S) );
          return S;
        }

    /// "Overriden" (specialization) so that we may have
    // that `declContextStack` possibility.
    // This is invoked by the various Traverse##CLASS##Decl() methods.
    bool TraverseDeclContextHelper(DeclContext *DC);

    /**
     * - See base class for TraverseDecl() & WalkUpFromDecl().
     */
    bool VisitDecl(Decl *D);

    /**
     * Specialized so that we can discard declarations from the very beginning
     * of translation units until we enter the main file.
     */
    bool TraverseTranslationUnitDecl(TranslationUnitDecl *decl);
    bool WalkUpFromTranslationUnitDecl(TranslationUnitDecl *decl);
    bool VisitTranslationUnitDecl(TranslationUnitDecl *D);

    bool WalkUpFromNamedDecl(NamedDecl *D);
    bool VisitNamedDecl(NamedDecl *D);

    bool WalkUpFromNamespaceDecl(NamespaceDecl *D);
    bool VisitNamespaceDecl(NamespaceDecl *D);

    bool WalkUpFromTypeDecl(TypeDecl *D);
    bool VisitTypeDecl(TypeDecl *D);

    bool WalkUpFromValueDecl(ValueDecl *D);
    bool VisitValueDecl(ValueDecl *D);

    bool WalkUpFromDeclaratorDecl(DeclaratorDecl *D);
    bool VisitDeclaratorDecl(DeclaratorDecl *D);

    bool WalkUpFromFieldDecl(FieldDecl *D);
    bool VisitFieldDecl(FieldDecl *D);

    bool TraverseFunctionDecl(FunctionDecl *decl);
    bool WalkUpFromFunctionDecl(FunctionDecl *decl);
    bool VisitFunctionDecl(FunctionDecl *decl);

    bool WalkUpFromVarDecl(VarDecl *D);
    bool VisitVarDecl(VarDecl *D);

    // bool VisitTagDecl(TagDecl *D);
    // bool VisitRecordDecl(RecordDecl *decl);
    // bool VisitCXXRecordDecl(CXXRecordDecl *D);
    // bool VisitEnumDecl(EnumDecl *decl);
    // bool VisitEnumConstantDecl(EnumConstantDecl *decl);
    // bool VisitFieldDecl(FieldDecl *decl);
    // bool VisitTypeAliasDecl(TypeAliasDecl *decl) ;
    // bool VisitTypedefNameDecl(TypedefNameDecl *decl);
    // bool VisitParmVarDecl(ParmVarDecl *decl) ;


  protected:
  };

} // plugin ns
} // clong ns

#endif // PIMPLIO_PLUGIN_PimplIOVisitor_HPP
