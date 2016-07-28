//
// Created by cadet on 7/18/16.
//
#ifndef FABIC_CLANG_AST_CONSUMER_HPP
#define FABIC_CLANG_AST_CONSUMER_HPP

#include <memory>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

# include "config.hpp"
# include "logging.hpp"

TL_NS_BEGIN
namespace clong {

  //namespace cl = ::clang;
  using namespace ::clang;

  /**
   * * [Parsing C++ (Andrew Birkett, 2001)](http://www.nobugs.org/developer/parsingcpp/)
   * * [Thoughts on the Insanity of C++ Parsing, David Beazley, 2012](http://www.swig.org/article_cpp.html)
   */
  class ClangASTConsumer
      : public ASTConsumer
  {
  public:
    using self_t = ClangASTConsumer;

  public:
    virtual ~ClangASTConsumer() override ;

    /// \inherit
    void HandleTranslationUnit(ASTContext& Ctx) override ;

    /**
     * Factory method for constructing new instances of this.
     *
     * @return
     */
    static std::unique_ptr< ClangASTConsumer > create();
  };

} // clong ns.
TL_NS_END

#endif //FABIC_CLANG_AST_CONSUMER_HPP
