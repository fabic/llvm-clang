
#include <nstd/Process.hpp>

using namespace nstd;

/**
 * MAIN !
 */
int main(int argc, char *argv[], char *env[])
{
  Process::abort();

  return 111;
}

