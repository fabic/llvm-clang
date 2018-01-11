#ifndef XPIMPLIO_PLUGIN_VISITORS_ValueDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_ValueDecl_HPP

#include "XNamedDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XValueDecl : public XNamedDecl {
  protected:
    const ValueDecl& Decl_;
  public:
    XValueDecl(PimplIOVisitor& PV,
               const ValueDecl& D);

    bool Visit() override;
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_ValueDecl_HPP

