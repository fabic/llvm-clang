#ifndef XPIMPLIO_PLUGIN_VISITORS_FieldDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_FieldDecl_HPP

#include "XDeclaratorDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XFieldDecl : public XDeclaratorDecl {
  protected:
    const FieldDecl& Decl_;
  public:
    XFieldDecl(PimplIOVisitor& PV,
               const FieldDecl& D);

    bool Visit() override;
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_FieldDecl_HPP

