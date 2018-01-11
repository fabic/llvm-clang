#include <cassert>
#include <llvm/Support/ErrorHandling.h>
#include <dude/util/Terminal.hpp>
#include "TypeResolver.hpp"

namespace clong {
namespace plugin {

  // ctor
  TypeResolver::TypeResolver(QualType QT, const PrintingPolicy& Policy)
    : type_(nullptr), decl_(nullptr), print_policy_(Policy)
  {
    type_ = QT.getTypePtr();
    assert(type_ != nullptr
      && "Can't handle situation where QualType has no Type* pointer.");
  }

  const Decl *
    TypeResolver::Resolve()
    {
      decl_ = ResolveImpl( type_ );
      return decl_;
    }

  const Decl *
    TypeResolver::ResolveImpl(const Type *T)
    {
      using TC = Type::TypeClass;
      TC tc = T->getTypeClass();
      switch( tc ) {
        case TC::Builtin: {
          const BuiltinType *B = cast<BuiltinType>( T );
          ((terrs().yellow() << "TypeResolver: Got a builtin: ")
            .white() << B->getName(print_policy_) << "\n")
            .reset();
          return nullptr;
        }
        //
        case TC::Elaborated: {
          const ElaboratedType *E = cast<ElaboratedType>( T );
          QualType QT = E->getNamedType();
          return ResolveImpl( QT );
        }
        // TagType/s
        case TC::Enum:
        case TC::Record: {
          const TagType *TT = cast<TagType>( T );
          const Decl *D = TT->getDecl();
          return D;
        }
        default:
          (((terrs().red().reverse() << "TypeResolver::resolve():")
            .red() << " No impl. for handling Type/s of class `")
            .white() << T->getTypeClassName() << "`.\n")
            .reset();
          return nullptr;
      }

      llvm_unreachable("This shalt not be (see default of switch-case).");
    }

  const Decl *
    TypeResolver::ResolveImpl(const QualType QT)
    {
      const Type *T = QT.getTypePtr();
      assert(type_ != nullptr
        && "Really: We can't handle situation where QualType has no Type* pointer.");
      return ResolveImpl( T );
    }

} // plugin ns.
} // clong ns.
