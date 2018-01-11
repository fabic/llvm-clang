#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "dude/util/Terminal.hpp"
#include "exceptions.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseDecl(const Decl *D)
    {
      TPush log("TraverseDecl");

      *log << "- kind: " << D->getDeclKindName() << tendl
           << "- at: " << D << tendl;

      Repo_.Add( D );

      switch(D->getKind())
      {
        case Decl::Namespace:
          if (!TraverseNamespaceDecl(cast<NamespaceDecl>(D)))
            return false;
          break;

        // Type < Tag
        case Decl::Enum:
        case Decl::Record:
          case Decl::CXXRecord:
            case Decl::ClassTemplateSpecialization:
              case Decl::ClassTemplatePartialSpecialization:
          return TraverseDeclContextIfAny( D );

        case Decl::TemplateTypeParm:
        case Decl::UnresolvedUsingTypename:
          throw not_implemented_error();

        // Type < TypedefName
        case Decl::TypeAlias:
        case Decl::Typedef:
          if (!TraverseTypedefNameDecl(cast<TypedefNameDecl>(D)))
            return false;
          break;
          // return TraverseDeclContextIfAny( D );

        default: {
          log.heading("Unknown decl. kind:")
            << tbold << twhite << ' ' << D->getDeclKindName() << tendl;
          const DeclContext *DC = dyn_cast<DeclContext>( D );
          if (DC != nullptr) {
            if (!TraverseDeclContext( DC ))
              return false;
          } // if is DC.
        } // default.
      } //switch.

      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseDeclContextIfAny(const Decl* D)
    {
      const DeclContext *DC = dyn_cast< DeclContext >( D );
      if (DC == nullptr)
        return false;
      return TraverseDeclContext( DC );
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseDeclContext(const DeclContext *DC)
    {
      TPush log("TraverseDeclContext");

      *log << "- kind: " << DC->getDeclKindName()
           << " ( " << DC << " ) "
           << tendl;

      if (DC == nullptr)
        throw clong_error("This ain't no DeclContext yo!");

      DCStack_.Push( DC );

      // See RecursiveASTVisitor<>::TraverseDeclContextHelper():
      //  "BlockDecls and CapturedDecls are traversed through
      //   BlockExprs and CapturedStmts respectively."
      // TODO: ^ Find out wtf that is about?

      for (auto *Child : DC->decls())
      {
        if ( isa<BlockDecl>(Child) ) {
          *log << tred << "[i] Skipping BlockDecl.\n";
          continue;
        }
        else if ( isa<CapturedDecl>(Child) ) {
          *log << tred << "[i] Skipping CapturedDecl.\n";
          continue;
        }

        bool stop = TraverseDecl( Child ) == false;
        // TODO
        // if (stop)
        //   return false;
      }

      auto dc = DCStack_.Pop();

      assert( dc == DC &&
          "Dude, for some obscur reason we popped a different DeclContext*"
          " off the DeclContextStack, and this isn't a good thing."  );

      return true;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

} // plugin ns.
} // clong ns.
