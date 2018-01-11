#ifndef XPIMPLIO_PLUGIN_VISITORS_NamedDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_NamedDecl_HPP

#include "XDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XNamedDecl : public XDecl {
  protected:
    const NamedDecl& Decl_;
  public:
    XNamedDecl(PimplIOVisitor& PV,
               const NamedDecl& D);

    bool Visit() override;

    /// Helper for obtaining the "identifier name" of a NamedDecl,
    /// iff it has one; else "<unnamed>" is returned.
    static const StringRef get_decl_name(const NamedDecl& D);
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_NamedDecl_HPP

