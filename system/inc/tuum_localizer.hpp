
#ifndef TUUM_LOCALIZER_H
#define TUUM_LOCALIZER_H

#include "tuum_system.hpp"

#include "loc_landmark_stream.hpp"

using namespace loc;

namespace tuum {

  class Localizer : public Subsystem
  {
  public:
    static Subsystem::TypeVar Type;
    static Subsystem::Type GetType() { return &Localizer::Type; }

  public:
    Localizer();

    void setup();
    void process();

    Subsystem::Type getType() { return Localizer::GetType(); }

    /** Coordinate translation methods **/
    Vec2i toAbsolute(Vec2i);

  protected:
    //TODO: Implement mapping

    LandmarkStream mLandmarkStr;
  };

}

#endif
