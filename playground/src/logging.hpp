#ifndef FABIC_LOGGING_HPP
#define FABIC_LOGGING_HPP

/** include/fabic/logging.hpp
 *
 * Basic logging abilities, based on Boost.Log v2
 *
 * \link http://www.boost.org/doc/libs/1_61_0/libs/log/doc/html/index.html
 */

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

# include "util/pointers.hpp"

#ifdef BOOST_LOG_TRIVIAL
#  define logtrace  BOOST_LOG_TRIVIAL(trace)
#  define logdebug  BOOST_LOG_TRIVIAL(debug)
#  define loginfo   BOOST_LOG_TRIVIAL(info)
#  define logwarn   BOOST_LOG_TRIVIAL(warning)
#  define logerror  BOOST_LOG_TRIVIAL(error)
#  define logfatal  BOOST_LOG_TRIVIAL(fatal)
#else
#  define logtrace (std::cerr << "TRACE: ")
#endif




/* !!! NOTE !!!
 *
 * Boost.log library... may break the build from time to time at link stage.
 *
 * If you get this sort of errors :
 *
 *    « undefined reference to `boost::log::v2s_mt_posix::trivial::logger::get()' »
 *
 * then it is actually about the sub-namepace `v2s_mt_posix`.
 *
 * Found the following from the manual :
 *
 * http://www.boost.org/doc/libs/1_61_0/libs/log/doc/html/log/rationale/namespace_mangling.html
 *
 * “ The library declares the boost::log namespace which should be used in
 *   client code to access library components. However, internally the library
 *   uses another nested namespace for actual implementation.
 *   [...] it often appears in compiler and linker errors and in some cases
 *         it is useful to know how to decode its name.
 *
 *   Currently, the namespace name is composed from the following elements:
 *
 *      <version><linkage>_<threading>_<system>
 *
 *   * The <version>   component describes the library major version.
 *                     It is currently v2.
 *   * The <linkage>   component tells whether the library is linked statically or dynamically.
 *                     It is s if the library is linked statically and empty otherwise.
 *   * The <threading> component is st for single-threaded builds and mt for multi-threaded ones.
 *   * The <system>    component describes the underlying OS API used by the library.
 *                     Currently, it is only specified for multi-threaded builds.
 *                     Depending on the target platform and configuration,
 *                     it can be posix, nt5 or nt6.
 *
 * As a couple quick examples :
 *
 *   “ v2s_st corresponds to v2 static single-threaded build of the library ;
 *     and v2_mt_posix - to v2 dynamic multi-threaded build for POSIX system API.
 *
 * [...] One common mistake is to build dynamic version of the library
 *       and not define BOOST_LOG_DYN_LINK or BOOST_ALL_DYN_LINK when building
 *       the application, so that the library assumes static linking by default. »
 *
 * Solution :
 *   - CMakeLists.txt : add_definitions(-DBOOST_LOG_DYN_LINK)
 *   - So that it passes `-DBOOST_LOG_DYN_LINK` as compiler argument.
 */

#endif // FABIC_LOGGING_HPP
