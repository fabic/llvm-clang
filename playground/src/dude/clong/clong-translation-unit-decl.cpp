#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  using namespace clang;


  // [overriden, entry point, invoked by Clang when it's our time]
  void
    Clong::HandleTranslationUnit(ASTContext& context)
    {
      TPush log ("HEY!");

      // FIXME: Can we (an instance of this Clong) be invoked several times for
      // FIXME: processing different TUs ?
      assert(Context_ == nullptr);

      Context_ = &context;

      try {
        if (true)
          InvestigateASTContextTypes(context);

        if (false)
          InitLMDB();

        if (true)
          InitPostgresDatabase();

        const TranslationUnitDecl *TU = context.getTranslationUnitDecl();

        if (!TraverseTranslationUnitDecl( TU ))
          throw std::runtime_error("TraverseTranslationUnitDecl( TU ) "
                                   "returned false ?!");

        if(false)
          OutputPreprocessorTrace( *log );

        if (true) {
          *log << "`-> getASTAllocatedMemory() -> "
               << context.getASTAllocatedMemory() << tendl;
          *log << "`-> getSideTableAllocatedMemory() -> "
               << context.getSideTableAllocatedMemory() << tendl;
        }
      }
      catch(const std::exception &ex) {
        *log << ex;
      }
      // We really have to stop all exceptions here, Clang won't handle these
      // and abort with a stacktrace.
      catch(...) {
        *log << tendl << tred << treverse << "(!) CAN'T BE (!)"
             << tred << " Caught an exception of an unknown kind (!)"
             << twhite << " (try-catch(...) {})" << tendl << tendl;
      }
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseTranslationUnitDecl(const TranslationUnitDecl *TU)
    {
      TPush log;

      // TODO: infer main file name, and save it.
      auto id = PQXX_.Insert( R"(
        INSERT INTO decl (kind, context_id, name, fq_name)
        VALUES ($1, NULL, $2, NULL)
        RETURNING id ;)", 1, "Hola! (TU)" );

      *log << "- TU id: " << id << tendl;

      // TODO: Artifact& Art =
      Repo_.Add(TU, ID);
      // TODO: have that stack push Artifacts, somehow?
      //       ^ see https://stackoverflow.com/a/7707953/643087
      //         about std::reference_wrapper
      // http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
      DCStack_.Push( TU );

      for (auto *Child : TU->decls())
      {
        // Skip Decl/s that aren't coming from the "main file".
        if (!isInMainFile( *Child )) {
          // Note that we do want to process those few decls that have no file
          // (typically some compiler builtin types that get injected early on
          // into translation units).
          const FileEntry* FE = getFileEntry( *Child );
          if (FE != nullptr) {
            log.heading(" TU ") << " Skipping decl. : "
                                << Child->getDeclKindName()
                                << '(' << Child << ')'
                                << " [ " << FE->getName() << " ] "
                                << tendl;
            continue;
          }
        }

        // See TraverseDeclContext() about this.
        if (isa<BlockDecl>(Child) || isa<CapturedDecl>(Child))
          throw std::runtime_error("Hummm... that was unexpected -_-");

        TraverseDecl( Child );
      }

      PQXX_.Commit();

      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

} // plugin ns.
} // clong ns.
