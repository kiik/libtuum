/** @file tuum_navigator.cpp
 *  Navigator system interface. Inserts landmarks from perception subsystem
 *  into map & generates additional navmesh data for path calculation.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 2 July 2017
 */

#include "syscore/MotionData.hpp"

#include "sensors/GPS.hpp"

#include "tuum_platform.hpp"

#include "tuum_localizer.hpp"
#include "tuum_navigator.hpp"

namespace tuum {

  int OctreeLandmarkMap::id_seq = 0;

  OctreeLandmarkMap::OctreeLandmarkMap():
    m_id(++OctreeLandmarkMap::id_seq),
    m_lm_n(0),
    m_gps_anchor_fixed(false)
  {

  }

  int OctreeLandmarkMap::init()
  {
    return 0;
  }

  /*
  int push(Landmark* lm)
  {
    Vec2i pos = lm->getPosition();

    if(!octree_in_bounds(&m_root, pos)) return -1;

    return 0;
  }*/

  bool OctreeLandmarkMap::isAnchorFixed() {
    return m_gps_anchor_fixed;
  }

  void OctreeLandmarkMap::updateAnchor(Vec2d gps) {
    m_gps_anchor = gps;
    m_gps_anchor_fixed = true;
    gps_get_rect(gps, {1000, 1000}, m_gps_rect);
  }

  json OctreeLandmarkMap::toJSON()
  {
    json out;

    out["id"] = m_id;
    out["anchor"] = {m_gps_anchor.x, m_gps_anchor.y};
    out["dim"] = {m_root.w, m_root.h};
    out["landmark_n"] = m_lm_n;

    json buf = json::array();
    Vec2d *ptr;
    for(int ix = 0; ix < 4; ++ix) {
      ptr = &(m_gps_rect[ix]);
      buf.push_back({ptr->x, ptr->y});
    }

    out["gps_bounds"] = buf;

    return out;
  }

}

namespace tuum {

  Subsystem::TypeVar Navigator::Type;

  Navigator::Navigator():
    Subsystem("Navigator")
  {

  }

  int Navigator::init()
  {
    return 0;
  }

  void Navigator::setup()
  {

  }

  void Navigator::process()
  {
    // New landmarks? update navmesh
    // New Path request? calculate path
  }

  int Navigator::feedLandmarks(LandmarkSet* lms)
  {

  }

  int Navigator::getLocalMaps(EnvMapPtrSet& out)
  {
    out.push_back(&mMap);
  }

}
