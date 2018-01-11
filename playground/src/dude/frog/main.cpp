/** Ich bin frog !
 *
 * Would declare "src/" <=> "dude/"
 *
 * ! alias parent context as "dude".
 *
 * Implicit (optional) scope switch.
 *
 * ! push main.cpp .
 *
 * Or :
 * ! push $:self .
 *
 * Instructs that the compiler flags used are for Clang,
 * and if building with, say GCC, some mapping shall be
 * done to match correct flags.
 *
 * ! using compiler flags llvm-clang.
 *
 * ! executable frog
 *     # main.cpp (this file) is implied.
 *   (
 *     hacks/hack_cxa_throw_exception.o
 *     boost_filesystem-mt-d
 *     boost_log-mt-d
 *     boost_program_options-mt-d
 *     boost_system-mt-d
 *     # Boost C++ : program_options [mt, mt-d, d]
 *   ) [
 *     -fPIE -std=c++1z -stdlib=c++
 *     -Os -glldb
 *     -Wall -pedantic
 *     -Wdocumentation
 *
 *     -Wl,--warn
 *     -fcolor-diagnostics
 *     -Wmissing-field-initializers
 *     -Wcovered-switch-default
 *     -Wnon-virtual-dtor
 *     -Wdelete-non-virtual-dtor
 *     -Woverloaded-virtual
 *   ].
 *
 * ! debug build => [
 *     -g
 *     -fno-omit-frame-pointer
 *     -fno-optimize-sibling-calls
 *   ].
 *
 * ! release build => [
 *     -O3 -g0
 *     -D_NDEBUG
 *     -gline-tables-only -glldb
 *   ].
 *
 * ! using llvm-clang compiler
 *     and debug enabled => [ -gline-tables-only ].
 *
 * ! using gnu-binutils-ld linker      => [ -Wl,--warn-unresolved-symbols ].
 * ! using gnu-binutils-ld-gold linker => [ -flto -Wl,-flto ].
 * ! using llvm-lld linker => [].
 *
 * ! libc is gnu-glibc => [].
 * ! libc is musl-libc => [].
 *
 * Tune debug format for LLDB if found in path :
 *
 * ! lldb binary found in path => [ s/-ggdb/-glldb ].
 *
 * Will switch context up a level, moving scope from the current
 * context 'dude/frog' to the parent context 'dude' (which was
 * previously declared as an alias for 'src'.
 *
 * ! pop.
 */

#include <iostream>
#include <fstream>
#include <memory>

# include "dude/frog/frog.hpp"

/**
 * ! executable: dumbster
 */
int main(int argc, const char *argv[])
{
  logtrace << "Hey, I'm " << argv[0];

  namespace fs = boost::filesystem;

  using namespace dude::frog;

  Frog frog;

  for(int n = 1; n < argc; n++)
  {
    auto path = fs::path( argv[n] );
    if (fs::is_directory(path)) {
      logdebug << "Directory: " << path;
      frog.collect_sources(path);
    }
    else if (fs::is_regular_file(path)) {
      logdebug << "File: " << path << " SKIPPING (TODO impl)";
    }
  }

  logtrace << "Good bye...";

  return 0;
}

/// Multi-line comment
/// - this is the second line of it
//  - and here the third with a missing 3rd slash
/// - the fourth line
////// - the fifth with many leading slashes
// //  - the sixth with a space between two '//'.
/// - the last line.
