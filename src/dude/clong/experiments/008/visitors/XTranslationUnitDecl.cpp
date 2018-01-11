#include <cassert>
#include "XTranslationUnitDecl.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::TraverseTranslationUnitDecl(TranslationUnitDecl *TU)
    {
      if (true) {
        for (auto *Child : TU->decls())
        {
          // Skip Decl/s that aren't coming from the "main file".
          if (!isInMainFile( *Child )) {
            // But we do want to process those few decls that have no file
            // (typically some compiler builtin types that get injected
            //  early on into translation units).
            const FileEntry* FE = getFileEntry( *Child );
            if (FE != nullptr) {
              TLOG("TU", "Skipping decl. : ")
                << Child->getDeclKindName()
                << ' ' << Child
                << ' ' << " ( " << FE->getName() << " ) "
                << "\n";
              continue;
            }
          }

          // See `Base::TraverseDeclContextHelper(DeclContext*)` about this :
          //   "BlockDecls and CapturedDecls are traversed through
          //    BlockExprs and CapturedStmts respectively."
          if (!isa<BlockDecl>(Child) && !isa<CapturedDecl>(Child))
            TraverseDecl( Child );
        }
      }

      //return Base::TraverseTranslationUnitDecl( TU );
      return true;
    }


  bool
    PimplIOVisitor::WalkUpFromTranslationUnitDecl(TranslationUnitDecl *decl)
    {
      /* note: we're not walking up here. */
      return VisitTranslationUnitDecl( decl );
    }


  bool
    PimplIOVisitor::VisitTranslationUnitDecl(TranslationUnitDecl *D)
    {
      assert(D != nullptr && "Can't be: `TranslationUnitDecl *D == nullptr` (!)");
      auto V = CreateX< XTranslationUnitDecl >( *D );
      return V->Visit();
    }


  // ctor
  XTranslationUnitDecl::XTranslationUnitDecl(
      PimplIOVisitor& PV,
      const TranslationUnitDecl& TUD)
    : XDecl(PV, TUD), TUD_(TUD)
  { /* noop */ }


  bool
    XTranslationUnitDecl::Visit() {
      TERRS("TranslationUnitDecl", Decl_.getDeclKindName(), &Decl_) << tendl;

      const ASTContext&    Context = TUD_.getParentASTContext();
      const SourceManager& SrcMgr  = Context.getSourceManager();
      const FileID FID = SrcMgr.getMainFileID();
      const FileEntry *FE = SrcMgr.getFileEntryForID( FID );

      if ( FE ) {
        terrs() << "  - file: " << tcyan << FE->getName()
                << treset << " ( "
                << twhite << FE->tryGetRealPathName()
                << tnormal << " )";
      }

      terrs() << treset << tendl;
      return true;
    }

} // plugin ns.
} // clong ns.
