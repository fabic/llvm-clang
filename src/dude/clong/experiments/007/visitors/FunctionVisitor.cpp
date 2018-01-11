#include "FunctionVisitor.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  // ctor
  FunctionVisitor::FunctionVisitor(const FunctionDecl *FD,
                                   PimplIOVisitor *pim)
    : PIMP_( pim ), FD_(FD)
  {
    assert(FD_ != nullptr && "Ouch! nullptr!");
    assert(PIMP_ != nullptr && "Ouch! nullptr!");
    /* noop */
  }


  bool
    FunctionVisitor::Visit()
    {
      (terrs().green() << "[FunctionDecl]["
        << FD_->Decl::getDeclKindName() << "] \n")
        .reset();

        {
          TypeResolver TR (FD_->getReturnType(), PIMP_->getPrintingPolicy());
          const Decl *decl = TR.Resolve();
          if (decl != nullptr) {
            (terrs().indent().pad().cyan()
              << "` visiting return type decl. (!! WARNING !! INFINITE RECURSION !!)...\n").reset();
            PIMP_->TraverseDecl( const_cast<Decl*>(decl) );
          }
        }

        {
          const TypeSourceInfo *TSI = FD_->getTypeSourceInfo();
          const QualType QT = TSI->getType();
          const Type *T = QT.getTypePtr();
          const FunctionProtoType *FPT = dyn_cast<FunctionProtoType>(T);
          assert(FPT != nullptr && "Can't handle this!");
        }

      return true;
    }

} // plugin ns.
} // clong ns.
