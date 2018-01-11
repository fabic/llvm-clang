#ifndef XPIMPLIO_PLUGIN_VISITORS_Decl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_Decl_HPP

#include "XBase.hpp"

// todo: #ifdef ... => error. ?
#undef TERRS
#define TERRS(BASE, DERIVED, STUFF) (                             \
    (((terrs().magenta().bold().reverse() << "[" << BASE << ']')  \
      .white().bold().reverse() << '[' << DERIVED << ']')         \
      .normal() << ' ' << STUFF).reset() )

namespace clong {
namespace plugin {

  using namespace clang;

  /**
   */
  class XDecl : public XBase {
  protected:
    const Decl& Decl_;
  public:
    explicit XDecl(PimplIOVisitor& PV,
                   const Decl& D);

    const Decl& getDecl() const { return Decl_; }

    const Decl *getDeclPtr() const { return &Decl_; }

    bool Visit() override;

    /// Don't forget to check if `isValid()/isInvalid()`.
    SourceRange getSourceRange() const override {
      return Decl_.getSourceRange();
    }

  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_Decl_HPP

