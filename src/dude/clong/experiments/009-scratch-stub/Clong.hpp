#ifndef _PIMPL_PLUGIN_Clong_HPP
#define _PIMPL_PLUGIN_Clong_HPP

#include <set>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>

namespace clong {
namespace plugin {

  using namespace clang;

  /**
   *
   */
  class Clong : public ASTConsumer {
  private:
    CompilerInstance &Instance;
    std::set<std::string> ParsedTemplates;

  public:
    /// Ctor
    Clong(CompilerInstance &Instance,
                          std::set<std::string> ParsedTemplates);

    ~Clong() override;

    /**
     * Entry point: We're run somehow when the/a trans. unit is available.
     */
    void HandleTranslationUnit(ASTContext& context) override;

  private:
    /**
     * <http://clang.llvm.org/doxygen/classclang_1_1ASTContext.html#a151571ff7d2efdb6b9b6026773916056>
     */
    void InvestigateASTContextTypes( ASTContext& context );
  };

} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_Clong_HPP
