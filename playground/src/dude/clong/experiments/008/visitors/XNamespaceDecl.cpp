#include <cassert>
#include "XNamespaceDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::WalkUpFromNamespaceDecl(NamespaceDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitNamespaceDecl( D );
    }


  bool
    PimplIOVisitor::VisitNamespaceDecl(NamespaceDecl *D)
    {
      assert(D != nullptr && "Can't be: `NamespaceDecl *D == nullptr` (!)");
      auto V = CreateX< XNamespaceDecl >( *D );
      return V->Visit();
    }


  // ctor
  XNamespaceDecl::XNamespaceDecl(PimplIOVisitor& PV,
                                 const NamespaceDecl& D)
    : XNamedDecl(PV, D),
      Decl_(D)
  { /* noop */ }


  bool
    XNamespaceDecl::Visit() {
      TERRS("NamespaceDecl", Decl_.Decl::getDeclKindName(), &Decl_);

      terrs().reset() << "\n";
      return true;
    }

} // plugin ns.
} // clong ns.
