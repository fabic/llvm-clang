
void first_func(int a, float b, unsigned c);

int main(int argc, char *argv[])
{

  int sum = 0;
  for(int i = 0; i<100; i++) {
    sum+=i;

    if (sum>50)
      goto loop_exit;
  }
loop_exit:



unroll_stuff:
  return 0;
}

/// Re-declaration.
void first_func(int a, float b, unsigned c);

void second_func(void (*func_ptr)(int, float, unsigned));

/**
 * HEY 3rd func. that is.
 */
void *third_func(void *obscur_ptr) {
  return static_cast<int*>(obscur_ptr) + 12;
}

bool fourth_func(int x, unsigned y, double z, char i, long j, float k) {
  return (x && y) * z;
}

namespace ns {
  class B;

  class A {
  protected:
    void do_something(char c, int i, float f);
  public:
    void do_something_else(const A * const a_cptr, A *const a_ptrc);
  };

  class B {
    int stack[1024];
  public:
    void make_go_with_this(A& a_ref);
  };
}

namespace impl {
  /// 5th func. that declares a local struct.
  ns::A fifth_function_with_inner_struct(ns::A* ptr)
  {
    struct InnerStructInFifthFunction {
      int a, b, c;
    } that_is_defined_immediately = { 1, 2, 3};

    return *ptr;
  }
} // impl ns.

enum a_named_enumeration : unsigned {
  E_SHIFT_1 = 1 >> 1,
  E_SHIFT_2 = 1 >> 2,
  E_SHIFT_3 = 1 >> 3
};

enum {
  POSITIVE, NEGATIVE, ZERO
};
