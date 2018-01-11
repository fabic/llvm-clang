#ifndef PIMPLIO_PLUGIN_TypeResolver_HPP
#define PIMPLIO_PLUGIN_TypeResolver_HPP

#include <clang/AST/Type.h>
#include <clang/AST/Decl.h>
// #include <tuple>

namespace clong {
namespace plugin {

  using namespace clang;

  // Forward decl.
  class Clong;

  /**
   * Helper class for working with (around) Clang's Type hierarchy.
   * Finality is to come up with the corresponding `Decl*` where applicable.
   *
   * * see `clang/AST/TypeLocNodes.def`
   * * and `clang/AST/TypeNodes.def`
   * * see `clang/AST/TypeLocVisitor.h`
   */
  class TypeResolver {
  private:
    const Type *type_;
    const Decl *decl_;
    // const PrintingPolicy& PrintPolicy_;
    Clong& Clong_;
  public:
    explicit TypeResolver(Clong& C);

    /// Returns the eventually resolved `Decl*` (i.e. `decl_`).
    const Decl* Resolve(const QualType QT);
    const Decl* Resolve(const TypeSourceInfo *TSI);
    const Decl* Resolve(const TypeLoc TL);

  private:
    /// The recursive counter-part of `Resolve()`.
    const Decl* ResolveImpl(const Type *T);

    /// Helper that basically invokes `ResolveImpl()` with the `Type *`
    /// that is "embedded" in the QualType (remember that that thing is
    /// a weird smart-pointer-like "thing").
    const Decl* ResolveImpl(const QualType QT);

    void ResolveBuiltinTypeLoc(const BuiltinTypeLoc TL);

  private:
    const char * GetTypeLocKindName(TypeLoc TL);
  };
} // plugin ns.
} // clong ns.

#endif // PIMPLIO_PLUGIN_TypeResolver_HPP
