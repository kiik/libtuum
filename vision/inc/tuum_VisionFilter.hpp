
#ifndef TUUM_VFM_H
#define TUUM_VFM_H

#include "tuum_cmv.hpp"

namespace tuum {

  class VisionFilter
  {
  public:
    VisionFilter();

    int apply(image_t);

  protected:
    CMV::Filter  mFilter;
    CMV::BlobSet mBlobs;

    Timer mDebugTmr;
  };

}

#endif
