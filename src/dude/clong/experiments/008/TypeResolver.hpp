#ifndef PIMPLIO_PLUGIN_TypeResolver_HPP
#define PIMPLIO_PLUGIN_TypeResolver_HPP

#include <clang/AST/Type.h>
#include <clang/AST/Decl.h>
// #include <tuple>

namespace clong {
namespace plugin {

  using namespace clang;

  /**
   * Helper class for working with (around) Clang's Type hierarchy.
   * Finality is to come up with the corresponding `Decl*` where applicable.
   */
  class TypeResolver {
  private:
    const Type *type_;
    const Decl *decl_;
    const PrintingPolicy& print_policy_;
  public:
    explicit TypeResolver(QualType QT, const PrintingPolicy& Policy);

    /// Returns the eventually resolved `Decl*` (i.e. `decl_`).
    const Decl * Resolve();

  private:
    /// The recursive counter-part of `Resolve()`.
    const Decl * ResolveImpl(const Type *T);

    /// Helper that basically invokes `ResolveImpl()` with the `Type *`
    /// that is "embedded" in the QualType (remember that that thing is
    /// a weird smart-pointer-like "thing").
    const Decl * ResolveImpl(const QualType QT);
  };
} // plugin ns.
} // clong ns.

#endif // PIMPLIO_PLUGIN_TypeResolver_HPP
