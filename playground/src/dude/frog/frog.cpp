
# include "dude/frog/frog.hpp"

namespace dude {
  namespace frog {

    /// Ctor.
    Frog::Frog()
    {
      _sources_list.reserve(2^16);
    }

    /// Dtor.
    Frog::~Frog() {}

    Frog::self
      Frog::collect_sources(fs::path from_directory)
      {
        logtrace << "Searching sources from directory " << from_directory;

        for (fs::directory_entry& de : fs::recursive_directory_iterator(from_directory))
        {
          if (! fs::is_regular_file(de)) {
            continue;
          }

          _sources_list.push_back(de.path());

          loginfo << "Found " << de.path();
        }

        logtrace << "Finished search in " << from_directory;

        return *this;
      }

  } // frog ns.
} // dude ns.

