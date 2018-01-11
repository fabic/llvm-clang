#include <algorithm>
#include "lmdb.hpp"

namespace clong {
namespace lmdb {

  NamespaceDecl *
    NamespaceDecl::create(StringRef name)
    {

      //FIXME: All of this should go to base NamedDecl. -__-

      size_t name_sz = name.size()+1;
      size_t sz = sizeof(NamespaceDecl) + name_sz;

      u8* buffer = new u8[ sz ];
      u8* data = buffer + sizeof(NamespaceDecl);

      NamespaceDecl* NS = new (buffer) NamespaceDecl();

      std::copy_n(name.data(), name_sz, data);

      data[ name_sz ] = '\0';

      Name_ = data - buffer;

      return NS;
    }


} // lmdb ns.
} // clong ns.
