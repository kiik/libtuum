
#ifndef LOC_LANDMARK_STREAM_H
#define LOC_LANDMARK_STREAM_H

#include <vector>

#include "Vec.hpp"

#include "loc_core.hpp"

using namespace tuum;

namespace loc {

  typedef std::vector<Vec2i> PointSet;

  /**
   *  Merge points into landmarks based on position and distance
   */
  class LandmarkStream
  {
  public:
    LandmarkStream();

    int stream(PointSet);

  protected:
    LandmarkSet mLandmarks;
  };

}

#endif
