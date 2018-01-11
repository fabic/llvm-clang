#ifndef XPIMPLIO_PLUGIN_VISITORS_NamespaceDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_NamespaceDecl_HPP

#include "XNamedDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XNamespaceDecl : public XNamedDecl {
  protected:
    const NamespaceDecl& Decl_;
  public:
    XNamespaceDecl(PimplIOVisitor& PV,
                   const NamespaceDecl& D);

    bool Visit() override;
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_NamespaceDecl_HPP

