
#ifndef LOC_LANDMARK_STREAM_H
#define LOC_LANDMARK_STREAM_H

#include <vector>

#include "Vec.hpp"

using namespace tuum;

namespace loc {

  typedef std::vector<Vec2i> PointSet;

  class Landmark
  {
  public:
    Landmark();

    int consumeMeasurement(Vec2i);

  protected:
    PointSet points;
  };

  typedef std::vector<Landmark> LandmarkSet;

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
