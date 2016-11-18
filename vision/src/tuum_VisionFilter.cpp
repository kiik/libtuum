
#include "tuum_vision.hpp"

#include "tuum_VisionFilter.hpp"

namespace tuum {

  CMV::ColorClass YUV_BALL = {51, 100, 148, 140, 123, 165}; // [59.5,110,160.5]

  CMV::ColorClass RGB_BALL = {130, 44, 30, 198, 112, 98};

  VisionFilter::VisionFilter()
  {
    uint32_t id;
    mFilter.addClass(YUV_BALL, id);

    mDebugTmr.start(1000);
  }

  int VisionFilter::apply(image_t in)
  {
    mBlobs.clear();
    vision::parse_blobs(in, mFilter, mBlobs);

    if(mDebugTmr.isTime()) {
      printf("%lu\n", mBlobs.size());
      mDebugTmr.start(1000);
    }
    return 0;
  }

}
