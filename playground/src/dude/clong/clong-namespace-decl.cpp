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

      auto ID = PQXX_.Insert( R"(
        INSERT INTO decl (kind, context_id, name, fq_name)
        VALUES ($1, NULL, $2, NULL)
        RETURNING id ;)", 2, D->getName().str() );

      *log << "- ID: " << ID << tendl;

      if (!TraverseDeclContext( D ))
        return false;

      return true;
    }


} // plugin ns.
} // clong ns.
