
#ifndef TUUM_VFM_H
#define TUUM_VFM_H

#include "tuum_cmv.hpp"

namespace tuum {

  class VisionFilter
  {
  public:
    VisionFilter();

    void initClassifiers();

    int apply(image_t);

    void addBlobDebugLayer(image_t);

  protected:
    CMV::UVFilter mFilter;
    CMV::BlobSet mBlobs, mBlobBuffer;

    Timer mDebugTmr;
  };

}

#endif
