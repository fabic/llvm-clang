
#include <memory>
#include <experimental/filesystem>

namespace dude {
namespace would_you_please {

  namespace fs = std::experimental::filesystem;

  std::unique_ptr<char *> read_file_contents(fs::path fileName);

} // would_you_please ns.
} // dude ns.
