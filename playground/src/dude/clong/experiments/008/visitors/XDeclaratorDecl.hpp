#ifndef XPIMPLIO_PLUGIN_VISITORS_DeclaratorDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_DeclaratorDecl_HPP

#include "XValueDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XDeclaratorDecl : public XValueDecl {
  protected:
    const DeclaratorDecl& Decl_;
  public:
    XDeclaratorDecl(PimplIOVisitor& PV,
                    const DeclaratorDecl& D);

    bool Visit() override;
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_DeclaratorDecl_HPP

