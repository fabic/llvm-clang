#include <cassert>
#include "XVarDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::WalkUpFromVarDecl(VarDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitVarDecl( D );
    }


  bool
    PimplIOVisitor::VisitVarDecl(VarDecl *D)
    {
      assert(D != nullptr && "Can't be: `VarDecl *D == nullptr` (!)");
      auto V = CreateX< XVarDecl >( *D );
      return V->Visit();
    }


  // ctor
  XVarDecl::XVarDecl(PimplIOVisitor& PV,
                     const VarDecl& D)
    : XDeclaratorDecl(PV, D),
      Decl_(D)
  { /* noop */ }


  bool
    XVarDecl::Visit() {
      TERRS("VarDecl", Decl_.getDeclKindName(), &Decl_);

      const TypeSourceInfo *TSI = Decl_.getTypeSourceInfo();
      assert(TSI != nullptr);

      terrs().blue() << " `";
      PrintType(TSI, Decl_.getIdentifier());
      terrs() << "` ";

      terrs().reset() << "\n";
      return true;
    }

} // plugin ns.
} // clong ns.
