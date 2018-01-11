#include "nstd/drm/kms.hpp"

namespace linux {
namespace drm {

  int
    kms::open(const char *node)
    {
      _control_dev.open(
          "/dev/dri/card0",
          File::OpenFlags::RDWR | File::OpenFlags::CLOEXEC
        );
    }

} // drm ns.
} // linux ns.
