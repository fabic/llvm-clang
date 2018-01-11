/*
 * clang++ -Xclang -load -Xclang lib/PrintFunctionNames.so -std=c++17 ../../test-003.cpp
 */

//
// Stray top-level comment.
//

struct simple_struct {
  int a;
};

/**
 * Some anonymous structure, immediately "instanciated".
 */
struct { int b; } inst_of_unnamed_struct;

int global1, global2;

class ThisIsAClass {
private:
  int var1;
};

template<typename T>
  class TemplatedClass1 {
    T t;
  };

template<typename T>
  T* templated_global_function(T& ref) { return nullptr; }

int some_global_func(int a, float b, ThisIsAClass& ref);

int main(int argc, char *argv[]) {

  return 0;
}

int some_global_func(int a, float b, ThisIsAClass& ref) {
  return 0;
}

namespace nstd {

  class Process {
    int pid;
  };

  class Thread {
    int tid;
  };

  namespace io {
    using file_descriptor = unsigned int;

    file_descriptor open(const char *fileName);
  }
}

namespace n {
  namespace m {
    /**
     * Class `C` comment.
     */
    class C {
      class Inner_Cls {
        int a, b, c;
      };
    private:
      bool firstPrivateMethod(int a) { return a+1024; }
      bool secondPrivateMethod(int a) { return a+2048; }
    protected:
      int firstProtectedMethod(const C *const cptr);
      int secondProtectedMethod(const C& cref);
    public:
      unsigned firstPublicMethod(C& ref, C* ptr);
      unsigned secondPublicMethod(nstd::io::file_descriptor fd);
    };
  }
}
