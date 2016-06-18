/**
 * boost_options_debug_encoding.cpp
 *
 * /me tryin' to debug a situation with Boost's program_options library failing
 * to “convert” program arguments (and anyway I don't want any conversion BTW,
 * but it insists on converting char[] to some std::wstring _with the unexpected
 * intent of converting it back to std::string, it appears).
 *
 * EDIT/2016-06-17: -_- the "bug" disappeared magically after I rebuilt Boost.
 * EDIT/2016-06-17: -_- and re-appeared, grrr!
 *
 * Buid with :
 *   - $CXX -g -O0 -rdynamic -std=c++14 -frtti -L. -Wl,-rpath=. -lboost_program_options -lunwind -ldl -lbfd -lpthread -o boost_options_debug_encoding boost_options_debug_encoding.cpp
 * FabiC 2016-06-16 https://github.com/fabic/libcsdbg/
 */

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

extern "C" {
    #include <libunwind.h>
}

#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <typeinfo>

// We pass -I.. for compiling so that we may do this
// (because include/... isn't "namespaced").
//#include "libcsdbg/include/tracer.hpp"

namespace fabic {
    namespace plays {

        namespace po   = boost::program_options;

        using program_arguments = po::variables_map;

        /**
         * MAIN which accepts
         */
        int main(program_arguments &args) {

            std::cout << "Hello world, you reached this point, can't believe it!"
                    " (libcsdbg test 1)" << std::endl;

            return 0;
        }


        /**
         * Resort to Boost's program_options lib.
         * for parsing command line arguments.
         *
         * @param  argc [description]
         * @param  argv [description]
         * @return      [description]
         */
        program_arguments
        process_program_arguments(int argc, const char *const argv[]) {
            namespace po = boost::program_options;

            // This is needed so for setting up the
            // “ conversion facet according to the user's selected locale ”
            // based on the LANG environment variable. Else an exception is
            // thrown from some deep abyss "character conversion failed".
            //   http://www.boost.org/doc/libs/1_61_0/doc/html/program_options/howto.html#idp308994400
            //   http://en.cppreference.com/w/cpp/locale/locale/global
            //std::locale::global(std::locale(""));
            std::locale::global(std::locale("en_US.UTF-8"));
            //std::locale::global(std::locale::classic());

            po::options_description desc("Allowed options");
            desc.add_options()
                    ("help", "produce help message")
                    ("address", po::value<char>(), "Hostname or IP address")
                    ("port", po::value<char>(), "TCP port number");

            po::positional_options_description posit;
            posit.add("extra", -1);

            po::variables_map args;

            auto parser = po::command_line_parser(argc, argv);

            parser
                    .options(desc)
                    .positional(posit);

            auto parsed_options = parser.run();

            po::store(parsed_options, args, true);

            po::notify(args);

            return args;
        }

    } // plays ns.

    namespace hack {

        // Forward decl.
        void abnormal_program_termination_handler();
        void normal_program_termination_atexit_handler();

        /** Entails that our program termination handler is registered early on
         * (at least earlier than it would have been from main()). */
        static const auto g_original_termination_handler
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
        void abnormal_program_termination_handler() {
            std::cerr << "Hey! that's terminate!" << std::endl;

            if (g_original_termination_handler != nullptr) {
                g_original_termination_handler();
            }
        }

        /** Normal program termination ( std::exit() ) handler.
         *
         * @link http://en.cppreference.com/w/cpp/utility/program/atexit
         */
        void normal_program_termination_atexit_handler() {
            std::cerr << "Hey! that's our std::exit() handler!" << std::endl;
        }

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
            static constexpr int tmp_procedure_name_str_length = 2048;
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

        CxaThrowBlackMagic::CxaThrowBlackMagic() : tmp_procedure_name_str(tmp_procedure_name_str_length, ' ') {
            this->orig_cxa_throw_func = (cxa_throw_func_ptr_type) dlsym(RTLD_NEXT, "__cxa_throw");

            if (this->orig_cxa_throw_func == nullptr)
                std::abort();
        }

        void
        CxaThrowBlackMagic::handle_thrown_exception(void *ex, std::type_info *info, void (*dest)(void *))
        {
            auto exception_name = info != nullptr ? this->demangle_cxx_type_name(info->name())
                                                  : std::string("no_exception_type_info");

            std::cerr << "Exception : " << exception_name << std::endl;

            this->attempt_stack_unwinding();

            std::cerr << std::endl;

            this->rethrow(ex, info, dest);
        }

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

        void
        CxaThrowBlackMagic::attempt_stack_unwinding() {
            unw_cursor_t cursor;
            unw_context_t uc;

            int status_code = 0xdeadbeef;

            status_code = unw_getcontext(&uc);
            if (status_code != 0)
                return;

            status_code = unw_init_local(&cursor, &uc);
            if (status_code != 0)
                return;

            boost::format fmtr("  [bt]: (%0u) [%#x] %s");

            int stack_frame_nb = 0;

            while (0 < (status_code = unw_step(&cursor))) {
                unw_word_t
                        ip = 0xdeadbeef,
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

                std::cerr << fmtr % stack_frame_nb % ip % routine << std::endl;

                stack_frame_nb++;
            }

            return;
        }

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

    } // hack ns.
} // fabic ns

/**
 * Invoqued by main() within a try-catch
 * (caught exception here is forwarded).
 */
int main_bis(int argc, const char *argv[])
{
    try {
        auto args = fabic::plays::process_program_arguments(argc, argv);
        auto exit_status = fabic::plays::main(args);
        return exit_status;
    }
    catch (std::exception& e) {
        std::cerr << "OUPS! Caught sthg: " << e.what() << std::endl;
        std::cerr << "      (forwarding exception)" << std::endl;
        throw e;
    }

    return 127;
}


/**
 * C-style main() actual entry point.
 */
int main(int argc, const char *argv[])
{
    // Init. libcsdbg's tracer thing :
    // using namespace csdbg;
    //
    // tracer *iface = tracer::interface();
    // if ( unlikely(iface == NULL) ) {
    //     std::cerr << "FAILED! couldn't initialize libcsdbg's tracer thing." << std::endl;
    //     return 126;
    // }

    try {
        return main_bis(argc, argv);
    }
    catch (std::exception& e) {
        std::cerr << "HEY! Caught an exception : " << e.what() << std::endl;
        std::cerr << "     (forwarding it, will quite probably end up handled by"
                     " std::terminate() somehow...)" << std::endl;
        throw e;
    }

    return 127;
}
