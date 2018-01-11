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

      std::string Name = D->getName().str();
      std::string SourceCode = getSourceCode( D->getSourceRange() ).str();

      *log << "- name: " << tcyan << Name << tnormal << tendl;
      *log << "- code: " << SourceCode << tendl;

      auto ID = PQXX_.Insert( R"(
        INSERT INTO decl (kind, context_id, name, fq_name, code)
        VALUES ($1, NULL, $2, NULL, $3)
        RETURNING id ;)", 3, Name, SourceCode );

      *log << "- ID: " << ID << tendl;

      const TypeSourceInfo *TSI = D->getTypeSourceInfo();
      assert(TSI != nullptr && "Woups!");

      TypeResolver TR ( *this );
      TR.Resolve( TSI );

      return true;
    }

} // plugin ns.
} // clong ns.
