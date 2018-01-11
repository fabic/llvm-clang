#include <cassert>
#include "XDeclaratorDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::WalkUpFromDeclaratorDecl(DeclaratorDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitDeclaratorDecl( D );
    }


  bool
    PimplIOVisitor::VisitDeclaratorDecl(DeclaratorDecl *D)
    {
      assert(D != nullptr && "Can't be: `DeclaratorDecl *D == nullptr` (!)");
      auto V = CreateX< XDeclaratorDecl >( *D );
      return V->Visit();
    }


  // ctor
  XDeclaratorDecl::XDeclaratorDecl(PimplIOVisitor& PV,
                                   const DeclaratorDecl& D)
    : XValueDecl(PV, D),
      Decl_(D)
  { /* noop */ }


  bool
    XDeclaratorDecl::Visit() {
      TERRS("DeclaratorDecl", Decl_.getDeclKindName(), &Decl_) << tendl;

      // StringRef Code = getSourceCode();
      // terrs() << "CODE: " << Code;

      const TypeSourceInfo *TSI = Decl_.getTypeSourceInfo();
      assert(TSI != nullptr);

      terrs() << "  - type: `" << tblue;
      PrintType(TSI, Decl_.getIdentifier());
      terrs() << treset << "` ";

      terrs() << treset << "\n";
      return true;
    }

} // plugin ns.
} // clong ns.
