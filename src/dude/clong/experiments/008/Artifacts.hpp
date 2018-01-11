#ifndef PIMPLIO_PLUGIN_ARTIFACTS_H
#define PIMPLIO_PLUGIN_ARTIFACTS_H

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/SmallVector.h>
#include <clang/AST/Decl.h>

namespace clong {
namespace artifacts {

  using llvm::StringRef;
  using llvm::SmallVector;

  namespace cl = clang;

  class decl {
    /* must have no properties. */
    const cl::Decl *decl_;
  };

  class decl_context : public decl {
    const cl::DeclContext *context_;
  };

  class function : public decl_context {
    StringRef name_;
    std::string prototype_str;
    const cl::Decl *returns_;
    SmallVector< decl*, 24 > arguments_;
  };

} // artifacts ns.
} // clong ns.

#endif // PIMPLIO_PLUGIN_ARTIFACTS_H
