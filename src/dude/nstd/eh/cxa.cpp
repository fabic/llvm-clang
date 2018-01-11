#include <nstd/eh/cxa.hpp>
#include <nstd/c/assert.h>

namespace nstd {
namespace cxxabi {

  using namespace unwind;

  extern "C" {

    void *
      __cxa_begin_catch(void * unwind_arg) noexcept
      {
        return nullptr;
      }


    ReasonCode
      __gxx_personality_v0(UnwindState state,
                           Exception*  unwind_exception,
                           Context*    context)
      {
        return URC_FATAL_PHASE1_ERROR ;
      }

  } // extern "C" //

} // cxxabi ns.
} // nstd ns.

