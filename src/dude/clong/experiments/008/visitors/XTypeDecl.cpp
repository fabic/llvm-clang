#include <cassert>
#include "XTypeDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::WalkUpFromTypeDecl(TypeDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitTypeDecl( D );
    }


  bool
    PimplIOVisitor::VisitTypeDecl(TypeDecl *D)
    {
      assert(D != nullptr && "Can't be: `TypeDecl *D == nullptr` (!)");
      auto V = CreateX< XTypeDecl >( *D );
      return V->Visit();
    }


  // ctor
  XTypeDecl::XTypeDecl(PimplIOVisitor& PV,
                       const TypeDecl& D)
    : XNamedDecl(PV, D),
      Decl_(D)
  { /* noop */ }


  bool
    XTypeDecl::Visit() {
      TERRS("TypeDecl", Decl_.getDeclKindName(), &Decl_) << tendl;

      const Type *T = Decl_.getTypeForDecl();
      if (T != nullptr) {
      }
      // else {} log sthg

      terrs() << " - type: `" << tblue;
      PrintType(T, Decl_.getIdentifier());
      (terrs() << trset << "` ");

      StringRef Code = getSourceCode();
      //terrs().endl() << "CODE: \n" << Code;

      terrs().reset() << "\n";
      return true;
    }

} // plugin ns.
} // clong ns.
