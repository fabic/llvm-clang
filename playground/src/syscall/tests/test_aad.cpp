
class Foo {
  char str[64];
  int  integer;
  long longer_integer;
  void *some_anonymous_pointer;
};

/**
 * MAIN !
 */
int main(int argc, char *argv[], char *env[])
{
  auto foo1 = new Foo();

  return argc;
}
