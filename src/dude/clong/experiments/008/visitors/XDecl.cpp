#include <cassert>
#include "XDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::VisitDecl(Decl *D)
    {
      assert(D != nullptr && "Can't be: `Decl *D == nullptr` (!)");
      auto V = CreateX< XDecl >( *D );
      return V->Visit();
    }


  // ctor
  XDecl::XDecl(PimplIOVisitor& PV, const Decl& D)
    : XBase( PV ), Decl_(D)
  {
    /* noop */
  }


  bool
    XDecl::Visit() {
      TERRS("(!!) Decl (!!)", Decl_.getDeclKindName(), &Decl_);

      StringRef Code = getSourceCode();
      terrs() << "CODE: " << Code;

      terrs().endl();
      return true;
    }

} // plugin ns.
} // clong ns.
