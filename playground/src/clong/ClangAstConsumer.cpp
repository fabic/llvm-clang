//
// Created by cadet on 7/18/16.
//

# include "fabic/clong/ClangAstConsumer.hpp"

TL_NS_BEGIN
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
TL_NS_END