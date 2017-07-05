/** @file tuum_navigator.hpp
 *  @brief Navigator system processes perception data,
 *  generates navmesh & pathing data, handles high-level motion commands.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 2. June 2017
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
    Subsystem("Navigator"),
    gLoc(nullptr),
    m_init(false)
  {

  }

  int Navigator::init()
  {
    return 0;
  }

  void Navigator::setup()
  {
    int err_flag = 0;

    if(getSubsystemHandle<tuum::Localizer*>(Localizer::GetType(), gLoc) > 0) {
      RTXLOG("Localizer present.");
    } else err_flag = -1;

    if(err_flag < 0) {
      RTXLOG(tuum::format("Error - Missing module. (code=%i)", err_flag), LOG_ERR);
    } else {
      m_init = true;
    }
  }

  void Navigator::process()
  {
    if(!m_init) return;

    // New landmarks? update navmesh
    // New Path request? calculate path

    if(m_ctx.flags != 0) {
      localized_pose_t pose;

      if(gLoc->getLocalPose(pose) >= 0) {
        Vec2i dP(0, 0);
        float dO = 0.0;

        if(m_ctx.flags & NAV_SET_POS) dP = m_ctx.tPos - pose.coord;
        if(m_ctx.flags & NAV_SET_ORI) dO = m_ctx.tOri - pose.orient;
      }

    }
  }

  int Navigator::navTo(const Vec2i& pos)
  {
    m_ctx.tPos = pos;
    m_ctx.flags = NAV_SET_POS;
  }

  int Navigator::navTo(const Vec2i& pos, const float& ori)
  {
    m_ctx.tPos = pos;
    m_ctx.tOri = ori;
    m_ctx.flags = NAV_SET_POS | NAV_SET_ORI;
  }

  int Navigator::feedLandmarks(LandmarkSet* lms)
  {

  }

  int Navigator::getLocalMaps(EnvMapPtrSet& out)
  {
    out.push_back(&mMap);
  }

}
