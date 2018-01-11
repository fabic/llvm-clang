#ifndef PIMPLIO_PLUGIN_VISITORS_XBase_HPP
#define PIMPLIO_PLUGIN_VISITORS_XBase_HPP

#include <clang/AST/Decl.h>
#include "../TypeResolver.hpp"

namespace clong {
namespace plugin {

  class PimplIOVisitor;

#if 1
  // Forward decl. of class hierarchy (informational).
  class XDecl;
  class   XNamedDecl;
  class     XTypeDecl;
// class       TagDecl;
// class         EnumDecl;
// class         RecordDecl;
// class           CXXRecordDecl;
// class           ClassTemplateSpecializationDecl;
// class             ClassTemplatePartialSpecializationDecl;
// class       TypedefNameDecl;
// class         TypeAliasDecl;
// class         TypedefDecl;
  class     XValueDecl;
  class       XDeclaratorDecl;
// class         FieldDecl;
  class         XFunctionDecl;
// class           CXXMethodDecl;
// class             CXXConstructorDecl;
// class             CXXConversionDecl;
// class             CXXDestructorDecl;
  class         XVarDecl;
// class           ParamVarDecl;
#endif

  /**
   * Base class for those "visitors".
   *
   * TODO: Have a hierarchy for Type/s (?)
   * TODO: Renames as Artifact ?
   */
  class XBase {
  protected:
    PimplIOVisitor& Pimp_;
  protected:
    explicit XBase(PimplIOVisitor& PV);
    virtual ~XBase() {}
    virtual bool Visit() =0;

  protected:

    /// Return the/a SourceRange for this type/declaration.
    virtual SourceRange getSourceRange() const {
      return SourceRange();
    }

    virtual StringRef getSourceCode(SourceRange SR = SourceRange()) const;

    // todo: const FileEntry* getFileEntry() const;
    const FileEntry* getFileEntry(const Decl& D) const;

    void PrintType(const TypeSourceInfo* TSI, StringRef PlaceHolder);
    void PrintType(const QualType QT, StringRef PlaceHolder);
    void PrintType(const Type* T, StringRef PlaceHolder);

    void PrintType(const TypeSourceInfo* TSI, const IdentifierInfo* I);
    void PrintType(const QualType QT, const IdentifierInfo* I);
    void PrintType(const Type* T, const IdentifierInfo* I);

  public:
  };

} // plugin ns.
} // clong ns.

#endif // PIMPLIO_PLUGIN_VISITORS_XBase_HPP
