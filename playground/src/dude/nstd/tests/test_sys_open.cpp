
#include <nstd/Process.hpp>

using namespace nstd;

/**
 * MAIN !
 */
int main(int argc, const char *argv[], const char *env[])
{
  File StdOut = Process::StdOut();

  StdOut.write("Hello! I'm ");
  StdOut.writeln(argv[0]);

  for(int i = 1; i < argc; i++)
  {
    auto pathName = argv[i];
    StdOut << "Arg.: " << pathName << "\n";
    File file(pathName);
  }

  StdOut << "Bye.\n";

  return 111;
}

