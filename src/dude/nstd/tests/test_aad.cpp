
class Foo {
  char str[64];
  mutable int  integer = 7;
  long longer_integer;
  void *some_anonymous_pointer;

public:
  int get() const { return integer++; }
};

/**
 * MAIN !
 */
int main(int argc, char *argv[], char *env[])
{
  auto foo1 = new Foo();

  // -_- compiler is smart enough to compute this at compile-time,
  //     ->get() is never invoked.
  return argc + foo1->get() + foo1->get();
}
