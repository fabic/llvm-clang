#ifndef XPIMPLIO_PLUGIN_VISITORS_FunctionDecl_HPP
#define XPIMPLIO_PLUGIN_VISITORS_FunctionDecl_HPP

#include "XDeclaratorDecl.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class XFunctionDecl : public XDeclaratorDecl {
  protected:
    const FunctionDecl& FD_;
    //
    // definitions
    // declaration/s?
    // prototype
    //   - return type/s
    //   - arguments
    //   - attributes
    // overrides
    // calls & called by.
    // comments
    // canonical name :
    //   - project ::: canonical prototype.
    //     ex. lmbdb:::lmdb_open(...) : lmdb_connection*.
    //
  public:
    XFunctionDecl(PimplIOVisitor& PV,
                  const FunctionDecl& FD);

    bool Visit();
  };

} // plugin ns.
} // clong ns.

#endif // XPIMPLIO_PLUGIN_VISITORS_FunctionDecl_HPP

