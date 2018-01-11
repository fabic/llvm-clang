/**
 * TEST 007
 */

/// Stray comments are lost.


/// MAIN (!) [forward decl.]
int main(int argc, char *argv[]);

/// `foo()` is declared, but not defined here.
int foo();

/// Namespace '::test'
namespace test {

  /// Namespace '::test::classes' opened in advance for
  /// the purpose of forward declarations.
  namespace classes {

    /// Forward decl. of class '::test::classes::Foo'
    class Foo;

  } // classes ns

  /// Sub-namespace '::test::functions'.
  namespace functions {

    /// An 'using' style alias for 'classes::Foo'
    /// (that was previously forward declared btw.).
    using Fow = classes::Foo;
    using Fow_c = const Fow;
    using Fow_cptr = Fow_c *;
    using Fow_cptr_c = Fow_cptr const;

    /// A `static void ()(void);` function declaration.
    static void compute_magic_numbers(void);

    /// Function decl. for `Fow& do_sthg_with_that_foo(const Foo&);`
    Fow& do_sthg_with_that_foo(const classes::Foo& foo);

  } // functions ns.

  /// Sub-namespace '::test::classes'.
  namespace classes {

    /// CLASS '::test::classes::Foo'.
    class Foo {
    private:
      int field1, field2;
      long field3, field4;
    protected:
      char str[255];
    public:
      /// Would be '::test::classes::Foo::BitField_t'
      union BitField_t {
        unsigned raw;
        struct {
          unsigned two:2;
          unsigned three:3;
          unsigned four:4;
          unsigned five:5;
        } pieces;
      }
      /// Can we do that (leaving a comment in this way) ??
      bits;
    };

  } // classes ns.


  /// "Out-of-line" definition of '::test::functions::compute_magic_numbers(void)'
  static void
    functions::compute_magic_numbers(void)
    {
      return;
    }

} // test ns.


/// Anonymous namespace.
namespace {
  bool g_bool = false;
  short g_short = 0;
} // <anonymous> ns.

/// And another <anonymous> namespace.
namespace {
  unsigned short g_ushort = 1;
  unsigned g_unsigned = -1;
} // <anonymous> ns.




/// main() definition at "global" scope.
int
  main(int argc, char *argv[])
  {
    using namespace ::test;

    classes::Foo feew;

    classes::Foo& fiiw =
      functions::do_sthg_with_that_foo( feew );
    return 0;
  }




/// Re-opening namespace '::test' at bottom of file,
/// for some definitions.
namespace test {

  /// Re-opening namespace '::test::functions' (bottom of file).
  namespace functions {

    /// Differing definition for func. `Fow& ...`.
    classes::Foo&
      do_sthg_with_that_foo(const classes::Foo& foo)
      {
        return const_cast<classes::Foo&>( foo );
      }

    classes::Foo func_that_returns_a_plain_foo( const classes::Foo& foo) {
      return foo;
    }

  } // functions ns. (re-opened).

  typedef unsigned int uintx_ ;
  typedef unsigned char byte ;
  typedef byte u8;
  typedef unsigned short u16;
  typedef const u16 cu16;

  using pcu16 = cu16 * ;
  using integers = int[ 256 ];
  using buffer = u8[ 1024 ];

  using funky = decltype( main );
}

// EOF ;-
