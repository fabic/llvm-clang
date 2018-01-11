#ifndef _PIMPL_PLUGIN_ClongPluginASTAction_HPP
#define _PIMPL_PLUGIN_ClongPluginASTAction_HPP

#include <set>
#include <clang/Frontend/FrontendAction.h>

namespace clong {
namespace plugin {

  using namespace clang;

  /**
   *
   */
  class ClongPluginASTAction : public PluginASTAction
  {
  private:
    using Base = PluginASTAction ;

    /// temp. hard-coded PostgreSQL connection details.
    static constexpr const char *PQXXConnectionOptionsStr =
            "dbname = clong user = fabi password = haiku "
            " hostaddr = 127.0.0.1 port = 5432" ;

  private:
    std::set<std::string> ParsedTemplates;

  public:
    /// ctor
    explicit ClongPluginASTAction();

    /// dtor
    ~ClongPluginASTAction() override;

    ///
    std::unique_ptr< ASTConsumer > CreateASTConsumer(CompilerInstance &CI,
                                                     llvm::StringRef InFile)
                                                       override;

    /// Automatically run the plugin before/after the main AST action.
    PluginASTAction::ActionType getActionType() override;

    ///
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override;

    ///
    void PrintHelp(llvm::raw_ostream& ros);

    bool BeginInvocation(CompilerInstance &CI) override;
    bool BeginSourceFileAction(CompilerInstance &CI) override;
    void ExecuteAction() override;
    void EndSourceFileAction() override;
  };

} // plugin ns
} // clong ns


#endif // _PIMPL_PLUGIN_ClongPluginASTAction_HPP
