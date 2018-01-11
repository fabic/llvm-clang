/**
 * ~~! executable: dumbster~~ !! see main() for target def.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <list>

# include "dude/dumbster/parser.hpp"


/**
 * ! executable: dumbster
 */
int main(int argc, const char *argv[])
{
  logdebug << "Hey, I'm " << argv[0];

  using dude::dumbster::Parser;

  try {
    for(int n = 1; n < argc; n++)
    {
      auto fileName = argv[n];
      logdebug << "~ ~ ~" << " FILE `" << fileName << "` ~ ~ ~";
      Parser parser (fileName);
      //parser.dev_parse();
      parser.parse();
      parser.debug_print_ast(std::cout);
    }
  }
  catch(dude::ex::yet_undefined_exception yex) {
    logfatal << yex.text();
  }

  logdebug << "Good bye...";

  return 0;
}
