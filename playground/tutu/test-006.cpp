#include <type_traits>


/// Forward decl. of `B`.
class B;

/**
 */
class A {
  B& bref;

  explicit A(B& b) : bref( b ) {
    /* noop */
  }
};

using Aah = A;


namespace tutu {
  struct decl {
    //int a, b;
  };

  struct named_decl : public decl {
    long c, d;
  };

  struct type_decl : public named_decl {
    char str[255];
  };

  struct tag_decl : public type_decl {
    float reals[1024];
  };

  static_assert(std::is_pod<tag_decl>() == true, "Ain't no POD.");
  static_assert(std::is_standard_layout<decl>() == true, "Ain't no standard layout either.");
  static_assert(std::is_standard_layout<named_decl>() == true, "Ain't no standard layout either.");
  static_assert(std::is_standard_layout<type_decl>() == true, "Ain't no standard layout either.");
  static_assert(std::is_standard_layout<tag_decl>() == true, "Ain't no standard layout either.");
}
