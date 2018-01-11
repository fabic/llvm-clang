#ifndef _DUDE_DRM_DRM_H
#define _DUDE_DRM_DRM_H

namespace linux {
namespace drm {

  class kms {
    protected:
      File _control_dev;
    public:
      kms();
      int open(const char *node);
  };

} // drm ns.
} // linux ns.

#endif // _DUDE_DRM_DRM_H

