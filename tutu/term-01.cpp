/**
 * clang++ -std=c++1z -Isrc tutu/term-01.cpp src/dude/util/Terminal.cpp src/clong/llvm/FormattedStream.cpp -o term-01 -lLLVMSupport -lpthread -lncursesw
 */
#include <dude/util/Terminal.hpp>

using namespace clong;

int main(int argc, char *argv[])
{
  terrs() << "HEY!\n";

  terrs().push();
    terrs() << "- What's up yo ?\n";
    terrs() << "- Cool, you?\n";

    terrs().push();
    terrs() << "What are one's last words"
               " if they will never be?\n"
               "I leave now, not with my own\n"
               "but thoses words that will exist beyond me.\n\n";
    (terrs().blue() << "W. Shakespeare\n").reset();
    terrs() << "---\n";
    terrs() << "12 Monkeys episode...\nSeason ...\nanother line...\n";
    terrs().blue().reverse() << "12345";
    terrs() << "\n67890\n";
    terrs().pop();
  terrs() << "- Okay, that was strange, is this a dream?\n";
  terrs().pop();

  terrs() << "BYE!\n";
  return 0;
}

