
#include <nstd/String.hpp>
#include <nstd/kernel/linux/Stream.hpp>

/**
 * MAIN !
 */
int main(int argc, char *argv[], char *env[])
{
  using kernel::Stream;
  using nstd::String;

  for(int i = 0; i < argc; i++) {
    auto str = argv[i];
    auto len = String::strlen(str);
    Stream::write(Stream::STDOUT, str, len);
    Stream::write(Stream::STDOUT, "\n", 1);
  }

  return 0;
}

