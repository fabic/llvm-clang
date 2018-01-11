#include <stdexcept>
#include <clang/AST/ASTContext.h>

#include "Clong.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::TraverseNamespaceDecl(const NamespaceDecl *D)
    {
      TPush log( D->getName() );

      if (!TraverseDeclContext( D ))
        return false;

      return true;
    }


} // plugin ns.
} // clong ns.
