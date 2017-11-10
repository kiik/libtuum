/** @file tuum_localizer.hpp
 *  @brief Calculates local pose & manages local-to-global transformations.
 *
 *  @author Meelik Kiik (kiik.meelik@gmail.com)
 *  @version 0.1
 *  @date 4. June 2017
 */

#ifndef TUUM_LOCALIZER_H
#define TUUM_LOCALIZER_H

#include "Vec.hpp"

#ifdef TUUM_HAL_GPS
#include "sensors/GPS.hpp"
#endif

#include "tuum_system.hpp"

#include "loc.hpp"
#include "loc_landmark_stream.hpp"

using namespace loc;

namespace tuum {

  class Navigator;

  struct localized_pose_t {
    Vec2i coord;
    float orient;
    uint32_t map_id;
    time_ms_t _t = 0;
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
    Vec2d toGlobalPosition(Vec2i);

    template<typename V>
    double globalDistanceTo(V& o) {
      return gps_calc_diff(m_gps, o);
    }

    int getLocalPose(localized_pose_t&);
    int getGlobalPosition(tuum::gps_t&);

  protected:
    bool m_init;
    Navigator* gNavi;

    tuum::gps_t m_gps;
    time_ms_t m_gps_t;

    localized_pose_t m_pose;
  };

}

#endif
