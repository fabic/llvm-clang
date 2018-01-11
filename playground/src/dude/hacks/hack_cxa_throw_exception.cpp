/** hack_cxa_throw_exception.cpp
 *
 * @since 2016-06-17
 *
 * @link http://stackoverflow.com/a/11674810
 */

#ifndef FABIC_CXA_THROW_HACK
#define FABIC_CXA_THROW_HACK
//      ^ This source _may indeed_ be included (instead of producing some .o
//        that you would have to somehow link to your binary by editing some
//        obscur build tool conf. file).

#include <dlfcn.h>
#include <cxxabi.h>

/* http://www.nongnu.org/libunwind/man/libunwind(3).html
 *   “ Normally, libunwind supports both local and remote unwinding
 *     (the latter will be explained in the next section). However, if you tell
 *     libunwind that your program only needs local unwinding, then a special
 *     implementation can be selected which may run much faster than the generic
 *     implementation which supports both kinds of unwinding. */
#ifndef UNW_LOCAL_ONLY
#   define UNW_LOCAL_ONLY
#endif

#include <libunwind.h>

#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include <cstdlib>

namespace dude {
  namespace hack {

#ifdef DUDE_CXA_HACK_INSTALL
    // Forward decl.
    void abnormal_program_termination_handler();
    void normal_program_termination_atexit_handler();

    /** Entails that our program termination handler is registered early on
     * (at least earlier than it would have been from main()). */
    static const std::terminate_handler
      g_original_termination_handler
        = std::set_terminate(abnormal_program_termination_handler);

    /** Likewise early on std::exit() handler registration. */
    static const bool g_atexit_handler_registered_ok =
      0 == std::atexit(normal_program_termination_atexit_handler);

    /**
     * Abnormal program termination ( std::terminate() ) handler.
     *
     * @link http://en.cppreference.com/w/cpp/error/set_terminate
     * @link http://stackoverflow.com/a/2445569
     */
    void abnormal_program_termination_handler()
    {
      std::cerr << "Hey! that's terminate!" << std::endl << std::endl;

      if (g_original_termination_handler != nullptr) {
        g_original_termination_handler();
      }
    }

    /** Normal program termination ( std::exit() ) handler.
     *
     * @link http://en.cppreference.com/w/cpp/utility/program/atexit
     */
    void normal_program_termination_atexit_handler()
    {
      std::cerr << "Hey! that's our std::exit() handler!" << std::endl;
    }
#else
    static const std::terminate_handler
      g_original_termination_handler = nullptr;
    static const bool g_atexit_handler_registered_ok = false;
    /// Absurd (!) Yeah it is.
    __attribute__((constructor))
      static void
        _pretend_used_unused_globals(void)
        {
          (void) g_original_termination_handler ;
          (void) g_atexit_handler_registered_ok ;
        }
#endif // DUDE_CXA_HACK_INSTALL


    /**
     * Yeah...
     */
    class CxaThrowBlackMagic {
      public:
        /**
         * See header `cxxabi.h` for the correct definition of `__cxa_throw()`.
         * `/usr/lib/gcc/x86_64-pc-linux-gnu/4.9.3/include/g++-v4/cxxabi.h`
         */
        typedef void (*cxa_throw_func_ptr_type)(
            void *exception,
            std::type_info *exception_type_info,
            void (*_dest)(void *)
            ) __attribute__ ((noreturn));

      private:
        /** The original (previous) __cxa_throw() actual implementation from
         * the CXX ABI lib. ; initialized from the ctor by a call to `dlsym()`. */
        cxa_throw_func_ptr_type orig_cxa_throw_func = nullptr;

        std::string tmp_procedure_name_str;
        static constexpr int tmp_procedure_name_str_length = 4096;

      public:
        /**
         * Ctor.
         * * See the `hack::g_cxa_throw_toolkit` global ;
         * * Note that Ctor will abort() in case it couldn't find out the
         *   address of the _previous (actual)_ `__cxa_throw()` function.
         */
        explicit CxaThrowBlackMagic();

        /**
         * Handle an invocation of __cxa_throw(), actually.
         * Processing is then handed over the actual `__cxa_throw()` impl. (hence the `noreturn`).
         */
        void handle_thrown_exception(void *ex, std::type_info *info, void (*dest)(void *)) __attribute__ ((noreturn));

        /**
         * Demangles the C++ type name through `abi::__cxa_demangle()`.
         *
         * * todo: pass the ex. eventually for providing some more info. along the generated name, like the mem. address ?
         */
        std::string demangle_cxx_type_name(const char *mangled_name);

      private:
        /**
         * Hand over processing to the actual __cxa_throw() impl.
         */
        void rethrow(
            void *exception,
            std::type_info *exception_type_info,
            void (*dest)(void *)
            ) __attribute__ ((noreturn)) {
          this->orig_cxa_throw_func(exception, exception_type_info, dest);
        }

        /**
         * @link http://www.nongnu.org/libunwind/man/libunwind(3).html
         */
        void attempt_stack_unwinding();
    };

    // ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~ ~~

    // [Constructor]
    CxaThrowBlackMagic::CxaThrowBlackMagic()
      : tmp_procedure_name_str(tmp_procedure_name_str_length, ' ')
    {
      this->orig_cxa_throw_func =
        (cxa_throw_func_ptr_type) dlsym(RTLD_NEXT, "__cxa_throw");

      if (this->orig_cxa_throw_func == nullptr)
        std::abort();
    }

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    void
      CxaThrowBlackMagic::handle_thrown_exception(void *ex,
                                                  std::type_info *info,
                                                  void (*dest)(void *))
      {
        auto exception_name = info != nullptr ? this->demangle_cxx_type_name(info->name())
          : std::string("no_exception_type_info");

        std::cerr << "Exception : " << exception_name << std::endl;

        this->attempt_stack_unwinding();

        std::cerr << std::endl;

        this->rethrow(ex, info, dest);
      }

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    std::string
      CxaThrowBlackMagic::demangle_cxx_type_name(const char *mangled_name)
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

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    void
      CxaThrowBlackMagic::attempt_stack_unwinding()
      {
        unw_cursor_t cursor;
        unw_context_t uc;

        int status_code = 0xdeadbeef;

        status_code = unw_getcontext(&uc);
        if (status_code != 0)
          return;

        status_code = unw_init_local(&cursor, &uc);
        if (status_code != 0)
          return;

        // boost::format fmtr("  [bt]: (%0u) [%#x] %s");

        int stack_frame_nb = 0;

        while (0 < (status_code = unw_step(&cursor)))
        {
          unw_word_t ip = 0xdeadbeef,
                     sp = 0xdeadbeef,
                     offset = 0xdeadbeef;

          status_code = unw_get_reg(&cursor, UNW_REG_IP, &ip);
          status_code |= unw_get_reg(&cursor, UNW_REG_SP, &sp);

          if (status_code != 0) {
            std::cerr << "Oups! couldn't fetch register";
          }

          auto& routine = this->tmp_procedure_name_str;

          status_code = unw_get_proc_name(
              &cursor,
              const_cast<char*>( routine.data() ),
              routine.capacity(),
              &offset
              );

          switch(status_code) {
            case UNW_ESUCCESS:
              routine = this->demangle_cxx_type_name(routine.c_str()).c_str();
              // ^ I hope with .c_str() to prevent eventual (probable?) move
              //   semantics here (don't want to loose our big buffer).
              break;
              // fixme: couldn't test error condition.
            case UNW_ENOMEM:
              routine += "...";
              routine.reserve( routine.capacity() << 1);
              // ^ double buffer size in advance.
              break;
            case UNW_EUNSPEC: // An unspecified error occurred.
            case UNW_ENOINFO: // Libunwind was unable to determine the name of the procedure.
              routine = "<unknown routine>";
              // no break, intentional.
            default:
              ;
              // man page says that
              //   “ in addition, unw_get_proc_name() may return any
              //     error returned by the access_mem() call-back
              //     (see unw_create_addr_space(3)) ”.
              // ^ and indeed it happens we get negative return status.
          }

          // std::cerr << fmtr % stack_frame_nb % ip % routine << std::endl;

          stack_frame_nb++;
        }

        return;
      }

    // ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~
    // ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~
    // ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~  x  ~~

#ifdef DUDE_CXA_HACK_INSTALL
    /**
     * Instantiate it eary on at program initialization.
     */
    CxaThrowBlackMagic g_cxa_throw_toolkit = CxaThrowBlackMagic();

    extern "C" {

      /** HACK: overrides the cxxabi implementation of `__cxa_throw()`
       * which is the low-level function that is used when you perform
       * a `throw new ...`.
       *
       * @link http://stackoverflow.com/a/11674810
       */
      void __cxa_throw(void *ex, std::type_info *info, void (*dest)(void *))
      {
        g_cxa_throw_toolkit.handle_thrown_exception(ex, info, dest);
      }

    } // extern "C"
#endif // DUDE_CXA_HACK_INSTALL

  } // hack ns.
} // dude ns.

#endif // FABIC_CXA_THROW_HACK
