
#include <cstdint>
#include <type_traits>
#include <llvm/Support/Casting.h>
#include <llvm/ADT/StringRef.h>

#include "dude/lmdb/lmdb++.h"

namespace clong {
  /**
   * * Having these structures adhere to the pseudo-concept StandardLayoutType
   *   was too constraining.
   * http://en.cppreference.com/w/cpp/concept/StandardLayoutType
   * `static_assert(std::is_standard_layout<NamedDecl>());`
   */
  namespace lmdb {

    using u64 = uint64_t;
    using u16 = uint16_t;
    using u8  = uint8_t;

    using llvm::StringRef ;

#ifdef PACKED_STRUCTURE
#error "Hey WTF !?"
#endif
#define PACKED_STRUCTURE __attribute__((packed))

    /**
     * It _really **is**_ a big positive number, and we partition
     * the domain ...
     *
     * Kind:
     *   - 0000 : Reserved for other meta-data, other custom/internal hierarchy ?
     *            ` have builtin types be hard-coded in here ? maybe not?
     *   - 1000 : Decl
     *   - 1100 : Type
     *            ` or have builtin types be ...
     *
     * Type: 8-bits Decl::Kind, Type::Kind, BuiltinType::Kind, etc.
     * Data: 20-bits for usage in derived classes.
     */
    struct Identifier {
      u64 kind:4; // Decl, Type, ???
      u64 type:8; // The actual Decl or Type kind (see Clang enums).
      // TODO: record size ??? O_O ?
      u64 data:20; // TODO: find a name for this void.
      u64 id:32; // 4 294 967 296 ~ 4 billion entries.

      /* OR:
       * - project id
       * - artifact kind :
       *     - "record" types: struct/union/class/..?/
       *     - "routines": function/method/closure
       *     - File: header vs. impl.
       *       ` main TU file ?
       */

      static constexpr u64 lower_bound_decls = 1L << 63 ;
      static constexpr u64 lower_bound_types = 3L << 62 ;

      Identifier() : kind(0b0000)
                   , type(0b0000'0000)
                   , data(0)
                   , id(-1)
      { /* noop */ }

      // Identifier(u64 raw)...
    };

    // static_assert(std::is_pod<Identifier>());
    static_assert(std::is_standard_layout<Identifier>());
    static_assert(sizeof(Identifier) == sizeof(u64));

    /**
     * "Abstract".
     */
    class Node {
    protected:
      uint8_t* base() { return reinterpret_cast<uint8_t*>( this ); }
      uint8_t* addr(u16 offset) {
        return base() + offset;
      }
      template<typename T>
        T* addressOf(u16 offset) {
          return reinterpret_cast<T*>( addr(offset) );
        }
    } PACKED_STRUCTURE;

    /**
     * Base class for declarations <-> clang::Decl.
     */
    class Decl : public Node {
    protected:
      Identifier ID_;
    } PACKED_STRUCTURE;

    static_assert(
        sizeof(Decl)
          == sizeof(Identifier)
          // + ... sizeof() all the members.
          , "We really need this clong::lmdb::Decl"
            " data structure to be packed.");

    // ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

    /**
     *
     */
    class NamedDecl : public Decl {
    protected:
      u16 Name_;
    public:
      /// ctor
      NamedDecl() : Name_(-1) {}

      ///
      StringRef getName() {
        char *str = addressOf<char>( Name_ );
        return StringRef( str );
      }
    } PACKED_STRUCTURE;

    static_assert(sizeof(NamedDecl) == sizeof(Decl)
                                     + sizeof(u16));

    // ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

    /*
     * NAMESPACES
     */
    class NamespaceDecl : public NamedDecl {
    public:
      NamespaceDecl *create(StringRef name);
    } PACKED_STRUCTURE;

    static_assert(sizeof(NamespaceDecl) == sizeof(NamedDecl));

#undef PACKED_STRUCTURE

  } // lmdb ns.
} // clong ns.
