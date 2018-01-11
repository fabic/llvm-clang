#include <cassert>
#include "XNamedDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::WalkUpFromNamedDecl(NamedDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitNamedDecl( D );
    }


  bool
    PimplIOVisitor::VisitNamedDecl(NamedDecl *D)
    {
      assert(D != nullptr && "Can't be: `NamedDecl *D == nullptr` (!)");
      auto V = CreateX< XNamedDecl >( *D );
      return V->Visit();
    }


  // ctor
  XNamedDecl::XNamedDecl(PimplIOVisitor& PV,
                                     const NamedDecl& D)
    : XDecl(PV, D),
      Decl_(D)
  { /* noop */ }


  bool
    XNamedDecl::Visit() {
      TERRS("NamedDecl", Decl_.getDeclKindName(), &Decl_) << tendl;

      const IdentifierInfo *II = Decl_.getIdentifier();
      // assert(II != nullptr && "Ouch! Got no IndentifierInfo* (nullptr)");
      // ^ actually it may happen, somehow...
      if (II != nullptr) {
        terrs() << "  - name: " << twhite << II->getName() << tendl;
      }

      // Does this ever happen?
      const NamedDecl *Other = Decl_.getUnderlyingDecl();
      if (Other != nullptr && Other != &Decl_) {
        terrs() << tred << " --underlying--> "
                << tnormal << get_decl_name(*Other);
      }

      terrs() << treset << "\n";
      return true;
    }

    const StringRef
      XNamedDecl::get_decl_name(const NamedDecl& D)
      {
        const IdentifierInfo *II = D.getIdentifier();
        if (II == nullptr)
          return "<unnamed>";
        return II->getName();
      }

} // plugin ns.
} // clong ns.
