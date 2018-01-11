#ifndef PIMPLIO_PLUGIN_VISITORS_XTranslationUnitDecl_HPP
#define PIMPLIO_PLUGIN_VISITORS_XTranslationUnitDecl_HPP

#include "XDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  /**
   */
  class XTranslationUnitDecl : public XDecl {
  protected:
    const TranslationUnitDecl& TUD_;
  public:
    XTranslationUnitDecl(PimplIOVisitor& PV,
                         const TranslationUnitDecl& TUD);

    bool Visit();
  };

} // plugin ns.
} // clong ns.

#endif // PIMPLIO_PLUGIN_VISITORS_XTranslationUnitDecl_HPP
