
#ifndef TUUM_LOCALIZER_H
#define TUUM_LOCALIZER_H

#include "Vec.hpp"

#include "tuum_system.hpp"

#include "loc_landmark_stream.hpp"

using namespace loc;

namespace tuum {

  class Navigator;

  struct localized_pose_t {
    Vec2i coord;
    float orient;
    uint32_t map_id;
  };

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

    int updateGlobalPosition();

    /** Coordinate translation methods **/
    Vec2i toAbsolute(Vec2i);

    int getLocalPose(localized_pose_t&);
    int getGlobalPosition(Vec2d&);

  protected:
    bool m_init;
    Navigator* gNavi;

    Vec2d m_gps;
    time_ms_t m_gps_t;

    localized_pose_t m_pose;
  };

}

#endif
