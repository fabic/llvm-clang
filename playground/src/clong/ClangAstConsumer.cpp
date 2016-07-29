
#include "ClangAstConsumer.hpp"

namespace clong {

  using namespace ::clang;

  // virtual btw.
  ClangASTConsumer::~ClangASTConsumer()
  { }


  void ClangASTConsumer::HandleTranslationUnit(ASTContext& Ctx)
  {
    //Ctx.getTranslationUnitDecl()->getSourceRange().getBegin()
    logtrace << "» Processing translation unit" ;
  }

  // static btw.
  std::unique_ptr< ClangASTConsumer >
    ClangASTConsumer::create()
  {
    return std::make_unique< ClangASTConsumer >();
  }

}
