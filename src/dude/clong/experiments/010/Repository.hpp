#ifndef _PIMPL_PLUGIN_Repository_HPP
#define _PIMPL_PLUGIN_Repository_HPP

#include <memory>

// #include <llvm/ADT/MapVector.h>
// ^ TEMP. imported :
#include "dude/llvm/MapVector.hpp"

namespace clang {
  class DeclContext;
  class Decl;
  class   TranslationUnitDecl;
  class NamedDecl;
  class   NamespaceDecl;

  class Type;
  class TypeLoc;
}

namespace clong {
namespace plugin {

  using namespace clang;

  // FIXME: Clang 5.0.1 -> 6.x : strange llvm::Type name conflict.
  using Type = clang::Type;

  /**
   *
   */
  class Artifact {
  private:
    const Decl* Decl_;

  public:
    explicit Artifact(const Decl* D) : Decl_(D) {}
  };


  /**
   *
   */
  class Repository {
  public:
    /// Key type may be a `const Decl*` or `const Type*`.
    using Key_t = size_t;
    using Map_t = MapVector< Key_t, Artifact > ;

  public:
    ///
    static Key_t KeyOf(const Decl *D) {
      return reinterpret_cast< size_t >( D );
    }

  private:
    Map_t Artifacts ;

  public:
    /// Return the `MapVector(&)` that stores the collected code artifacts.
    Map_t& getArtifactsMap() { return Artifacts ; }

    void Add(const Decl* D);
    void Add(const Type* T);
    void Add(const TypeLoc* T);
    // void AddDeclContext(const DeclContext *DC);
  };


  /**
   *
   */
  class DeclContextStack
          : public llvm::SmallVector< const DeclContext *, /* nElt= */ 32 > {
  public:
    const DeclContext* Push( const DeclContext *DC ) {
      push_back( DC );
      return current();
    }

    const DeclContext* Pop() {
      auto elt = current();
      pop_back();
      return elt;
    }

    const DeclContext* current() const {
      return back();
    }
  };


} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_Repository_HPP
