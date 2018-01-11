//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#ifndef DUDE_HACKS_ROUTINES_HPP
#define DUDE_HACKS_ROUTINES_HPP

#include <string>

namespace dude {
namespace hack {

  /**
   */
  class Routine {
  private:
    using Self = Routine ;

  public:
    /// Constructor.
    Routine();

    /// Destructor.
    ~Routine();

    std::string
      __attribute__ ((always_inline))
      operator()(unsigned frame = 2) {
        return AttemptRoutineNameLookup( frame );
      }

    std::string AttemptRoutineNameLookup(unsigned frame = 2);

  private:
    static std::string demangle_cxx_type_name(const char *mangled_name);
  };


} // hack ns.
} // dude ns.

#endif // DUDE_HACKS_ROUTINES_HPP
