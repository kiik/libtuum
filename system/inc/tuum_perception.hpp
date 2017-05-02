/** @file tuum_perception.hpp
 *  @brief Data association handler.
 *
 *  @author Meelik Kiik ( kiik.meelik@gmail.com )
 *  @version 0.1
 *  @date 2. May 2017
 */

#ifndef TUUM_PERCEPTION_H
#define TUUM_PERCEPTION_H

#include "loc_landmark_stream.hpp"

#include "tuum_system.hpp"

using namespace loc;

namespace tuum {

  class Perception : public Subsystem
  {
  public:
    static Subsystem::TypeVar Type;
    static Subsystem::Type GetType() { return &Perception::Type; }

  public:
    Perception();

    void setup();
    void process();

    Subsystem::Type getType() { return Perception::GetType(); }

    void processPoints(loc::PointSet);

    /** Landmark handling methods **/
    LandmarkStream* getLandmarkStream() { return &mLandmarkStr; }

  protected:
    LandmarkStream mLandmarkStr;
  };

}

#endif
