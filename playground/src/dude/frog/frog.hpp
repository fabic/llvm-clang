#ifndef _DUDE_FROG_FROG_H
#define _DUDE_FROG_FROG_H

#include <string>
#include <vector>
#include <boost/filesystem.hpp>

# include "dude/util/exceptions.hpp"
# include "dude/util/logging.hpp"

namespace dude {
namespace frog {

  namespace fs = boost::filesystem;
  using dude::ex::yet_undefined_exception;

  class Frog {
    public:
      using self = Frog& ;
    protected:
      static constexpr std::size_t initial_sources_list_capacity = 2^16;
      std::vector<fs::path> _sources_list;
    public:
      explicit Frog();
      virtual ~Frog();
      self collect_sources(fs::path from_directory);
  };

} // frog ns.
} // dude ns.

#endif // _DUDE_FROG_FROG_H
