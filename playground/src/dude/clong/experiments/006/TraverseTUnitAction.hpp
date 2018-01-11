#ifndef _PIMPL_PLUGIN_TraverseTUnitAction_HPP
#define _PIMPL_PLUGIN_TraverseTUnitAction_HPP

#include <set>
#include <clang/Frontend/FrontendAction.h>

namespace clong {
namespace plugin {

  using namespace clang;

  /**
   *
   */
  class TraverseTUnitAction : public PluginASTAction {
    using Base = PluginASTAction ;
  private:
    std::set<std::string> ParsedTemplates;

  public:
    /**
     * CTOR
     */
    explicit TraverseTUnitAction();
    ~TraverseTUnitAction() override;

    /**
     */
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   llvm::StringRef InFile) override;

    // Automatically run the plugin after the main AST action
    PluginASTAction::ActionType getActionType() override;

    /**
     */
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override;

    /**
     */
    void PrintHelp(llvm::raw_ostream& ros);

    bool BeginInvocation(CompilerInstance &CI) override;
    bool BeginSourceFileAction(CompilerInstance &CI) override;
    void ExecuteAction() override;
    void EndSourceFileAction() override;
  };

} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_TraverseTUnitAction_HPP
