
#include "tuum_logger.hpp"

#include "loc_landmark_stream.hpp"

namespace loc {

  LandmarkStream::LandmarkStream()
  {

  }

  int LandmarkStream::stream(PointSet points)
  {
    RTXLOG("#TODO");
    return -1;

    for(auto new_it = points.begin(); new_it != points.end(); new_it++) {

      for(auto it = mLandmarks.begin(); it != mLandmarks.end(); it++) {
        if(it->consumeMeasurement(*new_it) > 0) {
          break;
        }
      }

    }
  }

}
