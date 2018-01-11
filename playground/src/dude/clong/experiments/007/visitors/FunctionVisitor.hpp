#ifndef PIMPLIO_PLUGIN_VISITORS_FunctionVisitor_HPP
#define PIMPLIO_PLUGIN_VISITORS_FunctionVisitor_HPP

#include "Visitor.hpp"

namespace clong {
namespace plugin {

  using namespace clang;

  class PimplIOVisitor;

  /**
   */
  class FunctionVisitor : public Visitor {
  protected:
    PimplIOVisitor *PIMP_;
    const FunctionDecl *FD_;
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
  public:
    explicit FunctionVisitor(const FunctionDecl *FD, PimplIOVisitor *pim);

    bool Visit();
  };

} // plugin ns.
} // clong ns.

#endif // PIMPLIO_PLUGIN_VISITORS_FunctionVisitor_HPP

