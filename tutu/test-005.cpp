/**
 * File `tutu-005.cpp`
 *
 * That comment block is at "file scope".
 */

#if 0
#endif //0


namespace top {
  namespace inner1 {

      class Logger {
        unsigned fd;

      public:
        struct AStruct {
          unsigned foo:5;
          unsigned bar:4;
          union UN {
            unsigned char bytes[8];
            void *ptr;
          } u;
        };
      };

    namespace inner2 {

      namespace impl {
        class SpecificImplDetail {
          char padding[12];
        };
      }

      class ConcreteLogger : public Logger {
        const char *channel;
        const int level;

        using Impl = impl::SpecificImplDetail;
        Impl *impl;
      public:
        ConcreteLogger() : channel("default"), level(0), impl(nullptr) {}
      };
    }

    typedef inner2::ConcreteLogger ActualLogger;
  }
}

namespace top {
  namespace {
    class A;

    class A {

    };
  }
}



/**
 * [main description]
 *
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 */
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
