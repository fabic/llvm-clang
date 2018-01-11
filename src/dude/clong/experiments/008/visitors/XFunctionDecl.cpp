#include "XFunctionDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::TraverseFunctionDecl(FunctionDecl *decl)
    {
      return Base::TraverseFunctionDecl( decl );
    }

  bool
    PimplIOVisitor::WalkUpFromFunctionDecl(FunctionDecl *D)
    {
      /* note: we're not walking up here. */
      return VisitFunctionDecl( D );
    }


  bool
    PimplIOVisitor::VisitFunctionDecl(FunctionDecl *D)
    {
      assert(D != nullptr && "Can't be: `FunctionDecl *D == nullptr` (!)");
      auto V = CreateX< XFunctionDecl >( *D );
      return V->Visit();
    }


  // ctor
  XFunctionDecl::XFunctionDecl(PimplIOVisitor& PV,
                                           const FunctionDecl& FD)
    : XDeclaratorDecl(PV, FD),
      FD_(FD)
  { /* noop */ }


  bool
    XFunctionDecl::Visit()
    {
      TERRS("FunctionDecl", FD_.Decl::getDeclKindName(), &FD_)
        .endl()
        .push();

      const TypeSourceInfo *TSI = FD_.getTypeSourceInfo();
      assert(TSI != nullptr);

      terrs() << "  - type: `" << tcyan;
      PrintType(TSI, Decl_.getIdentifier());
      terrs() << tnormal << "`" << tendl;

      terrs() << "  - Return type: `" << tblue;
      PrintType(FD_.getReturnType(), "");
      terrs() << tnormal << '`' << tendl;

      {
        TypeResolver TR (FD_.getReturnType(), Pimp_.getPrintingPolicy());
        const Decl *decl = TR.Resolve();
        // if (decl != nullptr) {
        //   (terrs().indent().cyan()
        //     << "` visiting return type decl. (!! WARNING !! INFINITE RECURSION !!)...\n").reset();
        //   Pimp_.TraverseDecl( const_cast<Decl*>(decl) );
        // }
      }

#if 0
      {
        const TypeSourceInfo *TSI = FD_.getTypeSourceInfo();
        const QualType QT = TSI->getType();
        const Type *T = QT.getTypePtr();
        const FunctionProtoType *FPT = dyn_cast<FunctionProtoType>(T);
        if (FPT != nullptr) {
          // TODO //
        }
      }
#endif

      terrs().pop();
      return true;
    }

} // plugin ns.
} // clong ns.
