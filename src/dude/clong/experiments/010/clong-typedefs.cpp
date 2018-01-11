#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "TypeResolver.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseTypedefNameDecl(const TypedefNameDecl *D)
    {
      TPush log("Typedef / alias:");
      *log << "- name: " << tcyan << D->getName() << tnormal << tendl;
      *log << "- code: " << getSourceCode( D->getSourceRange() ) << tendl;

      const TypeSourceInfo *TSI = D->getTypeSourceInfo();
      assert(TSI != nullptr && "Woups!");

      TypeResolver TR ( *this );
      TR.Resolve( TSI );

      return true;
    }

} // plugin ns.
} // clong ns.
