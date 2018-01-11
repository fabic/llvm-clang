#ifndef _PIMPL_PLUGIN_Clong_HPP
#define _PIMPL_PLUGIN_Clong_HPP

#include <set>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>

#include "Repository.hpp"
#include "lmdb.hpp"
#include "PQXXHelper.hpp"
#include "PPCallbacksTracker.hpp"

// Forward decls
namespace clang {
  class DeclContext;
  class Decl;
  class   TranslationUnitDecl;
  class NamedDecl;
  class   NamespaceDecl;
  class   TypeDecl;
  class     TypedefNameDecl;
  struct PrintingPolicy;
}

namespace clong {
namespace plugin {

  using namespace clang;

  // FIXME: Clang 5.0.1 -> 6.x : strange llvm::Type name conflict.
  using Type = clang::Type;

  /**
   *
   */
  class Clong : public ASTConsumer {
  private:
    CompilerInstance &Instance;

    // FIXME (?): This is set by HandleTranslationUnit().
    const ASTContext *Context_ = nullptr;

    Repository Repo_ ;
    DeclContextStack DCStack_ ;

    std::set<std::string> ParsedTemplates;
    std::vector<CallbackCall> CallbackCalls;
    llvm::SmallSet<std::string, 4> Ignore;

    // todo: move to class lmdb::env.
    //std::string
    //const char *
    StringRef LMDBDatabasePathName = "./database.lmdb";
    ::lmdb::env LMDB_;

    PQXXHelper PQXX_ ;

  public:
    /// Ctor
    Clong(CompilerInstance &Instance,
                          std::set<std::string> ParsedTemplates,
                          const char *PQXXOptionsString);

    ~Clong() override;

    /**
     * Entry point: We're run somehow when the/a trans. unit is available.
     */
    void HandleTranslationUnit(ASTContext& context) override;

    /// Helper for getting the printing policy.
    /// (we'll probably need to instantiate a custom one since TypePrinter
    ///  outputs booleans as `_Bool` instead of `bool`.
    const PrintingPolicy& getPrintingPolicy() const;

    StringRef getSourceCode(SourceRange SR = SourceRange()) const;

  private:
    /**
     * <http://clang.llvm.org/doxygen/classclang_1_1ASTContext.html#a151571ff7d2efdb6b9b6026773916056>
     */
    void InvestigateASTContextTypes( ASTContext& context );

    /// Borrowed from Clang's tools-extra pp-trace.
    int OutputPreprocessorTrace( llvm::raw_ostream &OS );

    /// Helper for getting the FileEntry from whence this declaration is.
    const FileEntry* getFileEntry(const Decl& D) const;

    /// Helper that checks if the given declaration is from the "main file".
    bool isInMainFile(const Decl& D) const;

  private:
    bool TraverseTranslationUnitDecl(const TranslationUnitDecl *TU);
    bool TraverseDecl(const Decl *D);
    bool TraverseDeclContext(const DeclContext *DC);
    bool TraverseDeclContextIfAny(const Decl* D);
    bool TraverseNamespaceDecl(const NamespaceDecl *D);
    bool TraverseTypedefNameDecl(const TypedefNameDecl *D);

  private:
    bool InitLMDB();
    void InitPostgresDatabase();

  };

} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_Clong_HPP
