#include <cassert>
#include "XValueDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::WalkUpFromValueDecl(ValueDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitValueDecl( D );
    }


  bool
    PimplIOVisitor::VisitValueDecl(ValueDecl *D)
    {
      assert(D != nullptr && "Can't be: `ValueDecl *D == nullptr` (!)");
      auto V = CreateX< XValueDecl >( *D );
      return V->Visit();
    }


  // ctor
  XValueDecl::XValueDecl(PimplIOVisitor& PV,
                         const ValueDecl& D)
    : XNamedDecl(PV, D),
      Decl_(D)
  { /* noop */ }


  bool
    XValueDecl::Visit() {
      TERRS("ValueDecl", Decl_.getDeclKindName(), &Decl_);

      // const Type *T = Decl_.getTypeForDecl();
      // assert(T != nullptr);

      terrs().blue() << " `";
      PrintType(Decl_.getType(), Decl_.getIdentifier());
      terrs() << "` ";

      terrs().reset() << "\n";
      return true;
    }

} // plugin ns.
} // clong ns.
