#ifndef XPIMPLIO_PLUGIN_VISITORS_TypeDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_TypeDecl_HPP

#include "XNamedDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XTypeDecl : public XNamedDecl {
  protected:
    const TypeDecl& Decl_;
  public:
    XTypeDecl(PimplIOVisitor& PV,
              const TypeDecl& D);

    bool Visit() override;
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_TypeDecl_HPP

