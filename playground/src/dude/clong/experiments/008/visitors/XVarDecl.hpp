#ifndef XPIMPLIO_PLUGIN_VISITORS_VarDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_VarDecl_HPP

#include "XDeclaratorDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XVarDecl : public XDeclaratorDecl {
  protected:
    const VarDecl& Decl_;
  public:
    XVarDecl(PimplIOVisitor& PV,
             const VarDecl& D);

    bool Visit() override;
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_VarDecl_HPP

