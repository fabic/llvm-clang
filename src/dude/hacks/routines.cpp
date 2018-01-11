//===----------------------------------------------------------------------===//
//
// FABIC.NET / 2018-01-02
//
//===----------------------------------------------------------------------===//

#ifndef DUDE_HACKS_ROUTINES_IMPL_CPP
#define DUDE_HACKS_ROUTINES_IMPL_CPP
// ^
// ^ One _may_ want to include this .cpp file directly instead of fiddling
//   with whatever build system configuration he's using.
//

#include <stdexcept>

//#include <dlfcn.h>
#include <cxxabi.h>

#include <libunwind.h>

#include "dude/hacks/routines.hpp"

namespace dude {
namespace hack {

  Routine::Routine()
  { /* noop */ }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Routine::~Routine()
  { /* noop */ }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  std::string
    Routine::AttemptRoutineNameLookup(unsigned frame)
    {
      if (frame == 0) {
        throw std::invalid_argument(
            "Routine: `frame` argument may not be zero (!)");
      }

      unw_cursor_t  cursor;
      unw_context_t context;

      int status_code = 0xdeadbeef;

      status_code = unw_getcontext(&context);
      if (status_code != 0)
        return "<<routine name lookup failed (error code: 1)>>";

      status_code = unw_init_local(&cursor, &context);
      if (status_code != 0)
        return "<<routine name lookup failed (error code: 2)>>";

      int stack_frame_nb = 0;

      while (0 < (status_code = unw_step(&cursor)))
      {
        unw_word_t ip = 0xdeadbeef,
                   sp = 0xdeadbeef,
               offset = 0xdeadbeef;

        status_code = unw_get_reg(&cursor, UNW_REG_IP, &ip);
        status_code |= unw_get_reg(&cursor, UNW_REG_SP, &sp);

        if (status_code != 0) {
          return "<<routine name lookup failed (error code: 3)>>";
          // throw std::runtime_error("Routine::AttemptRoutineNameLookup(): "
          //                          "Oups! Couldn't fetch register (!)"    );
        }

        // Skip stack frames until we reach the one requested.
        frame--;
        if (frame > 0)
          continue;

        constexpr size_t routine_name_length = 128;
        std::string routine_name (routine_name_length, ' ');

        status_code = unw_get_proc_name(
            &cursor,
            const_cast<char*>( routine_name.data() ),
            routine_name.capacity(),
            &offset
          );

        switch(status_code) {
        // Good exit: we've got a possibly mangled C++ symbol, that
        //            we demangle.
        case UNW_ESUCCESS:
          return Self::demangle_cxx_type_name(routine_name.c_str());

        // There wasn't enough space for storing the function name,
        // we retry at most 6 times by doubling the size of our `routine_name`
        // string.
        case UNW_ENOMEM: {
          // double buffer size in advance.
          for(int i = 0; i < 6; i++)
          {
            size_t new_size = routine_name.capacity() << 1;
            routine_name.reserve( new_size );

            char *buffer = const_cast<char*>( routine_name.data() );
            size_t buffer_len = routine_name.capacity();

            status_code = unw_get_proc_name( &cursor,
                                             buffer,
                                             buffer_len,
                                             &offset    );
            // Good exit (bis).
            if (status_code == UNW_ESUCCESS)
              return Self::demangle_cxx_type_name(routine_name.c_str());
            // Do _not_ retry unless we're facing the
            // same "buffer ain't big enough" condition.
            else if (status_code != UNW_ENOMEM)
              break; // loop exit.
          }
          /* break; --> switch-case break,
                   `--> nope: fall through. */
        }

        case UNW_EUNSPEC: // An unspecified error occurred.
          return "<< unknown routine (error code: 4)>>";

                          // Libunwind was unable to determine
        case UNW_ENOINFO: // the name of the procedure.
          return "<< unknown routine (error code: 5)>>";

        default:
          /* noop */ ;

          // Man page says that
          //   “ in addition, unw_get_proc_name() may return any
          //     error returned by the access_mem() call-back
          //     (see unw_create_addr_space(3)) ”.
          //
          // ^ and indeed it does happen we get negative return status.
        }

        stack_frame_nb++;
      } // while-loop.
        //
        // ^ termination of this loop would mean unw_step() failed,
        //   and we're not handling this situation.
        //

      throw std::runtime_error( "Routine::AttemptRoutineNameLookup(): "
                                " We should not have reached this point." );
    }

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    // Static btw.
    std::string
      Routine::demangle_cxx_type_name(const char *mangled_name)
      {
        int status = 0xdeadbeef;

        // See cxxabi.h for the documentation.
        // PS: we're responsible for free()-ing the char* buffer.
        char *name = abi::__cxa_demangle(
            mangled_name,
            nullptr, // buffer
            nullptr, // buffer length
            &status  // demangling exit status
            );

        // Return as-is in case of error.
        if (name == nullptr || status != 0)
          return std::string(mangled_name);

        auto name_s = std::string(name);

        free(name);

        return name_s;

      }

} // hack ns.
} // dude ns.

#endif // DUDE_HACKS_ROUTINES_IMPL_CPP
