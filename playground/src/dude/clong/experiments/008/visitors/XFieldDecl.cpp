#include <cassert>
#include "XFieldDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::WalkUpFromFieldDecl(FieldDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitFieldDecl( D );
    }


  bool
    PimplIOVisitor::VisitFieldDecl(FieldDecl *D)
    {
      assert(D != nullptr && "Can't be: `FieldDecl *D == nullptr` (!)");
      auto V = CreateX< XFieldDecl >( *D );
      return V->Visit();
    }


  // ctor
  XFieldDecl::XFieldDecl(PimplIOVisitor& PV,
                         const FieldDecl& D)
    : XDeclaratorDecl(PV, D),
      Decl_(D)
  { /* noop */ }


  bool
    XFieldDecl::Visit() {
      TERRS("FieldDecl", Decl_.getDeclKindName(), &Decl_) << tendl;

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
