
#include <nstd/kernel/linux/Stream.hpp>


/**
 * MAIN !
 */
int main(int argc, char *argv[], char *env[])
{
  using kernel::Stream;
  using nstd::size_t;

  constexpr size_t bufferSize = 1023;

  char* buffer = new char[ bufferSize ];

  auto readCount = Stream::read(0, buffer, bufferSize);

  Stream::write(2, buffer, readCount);

  Stream::close(0);

  return 111;
}

