/** @file tuum_navigator.hpp
 *  @brief Navigator system processes perception data,
 *  generates navmesh & pathing data, handles high-level motion commands.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 2. June 2017
 */

#include "syscore/MotionData.hpp"

#ifdef TUUM_HAL_GPS
#include "sensors/GPS.hpp"
#endif

#include "tuum_platform.hpp"

#include "tuum_localizer.hpp"
#include "tuum_navigator.hpp"

namespace tuum {

  int OctreeLandmarkMap::id_seq = 0;

  OctreeLandmarkMap::OctreeLandmarkMap():
    EnvironmentMap(),
    m_id(++OctreeLandmarkMap::id_seq),
    m_lm_n(0)
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

  json OctreeLandmarkMap::toJSON()
  {
    json out;

    out["id"] = m_id;
    out["anchor"] = {m_anchor.lat, m_anchor.lon};
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


  NavmeshComposer::NavmeshComposer(Navigator *ptr):
    gNav(ptr)
  {

  }

  // Composer local rect area
  int NavmeshComposer::setStaticArea(rect_t rect)
  {
    m_rect = rect;
  }

  /*
  int NavmeshComposer::addBounds(map_pos_t pos, EnvironmentMap *env)
  {

  }
  */

  int NavmeshComposer::addVirtualBounds(const std::vector<Vec2i> *poly)
  {
    // mapToLocal
  }

  void NavmeshComposer::clearBounds()
  {

  }

  void NavmeshComposer::clearVirtBounds()
  {

  }


  Pather::Pather():
    m_valid_path(false),
    m_path_t(0),
    m_pathLogClk(1000),
    m_pos(0, 0), m_ori(0.0), m_pose_t(0),
    m_tPos(0, 0), m_tOri(0.0), m_tPose_t(0)
  {

  }

  void Pather::poseTick(Vec2i p, float o)
  {
    m_pos = p;
    m_ori = o;
    m_pose_t = millis();
  }

  Vec2d Pather::pathTick(Vec2i tP, float tO)
  {
    time_ms_t t = millis();
    Vec2d mvec(0.0, 0.0);

    if(t - m_pose_t <= 250) // 4Hz minimum pose update frequency
    {
      //TODO: implement pathfinding with intermediate waypoints
      // Generate abstract path (A*)
      // Generate kinematic visibility graph (V*)

      // Simulate path
      m_path.clear();
      m_path.push_back(tP);

      // Get motion vector to next point on path
      Vec2i p = m_path.front();
      mvec.x = p.x - m_pos.x;
      mvec.y = p.y - m_pos.y;

      double d = mvec.getMagnitude(), o = mvec.getOrientation(), dO = (o - m_ori);

      if(m_pathLogClk.tick()) {
        RTXLOG(tuum::format("pathing (%i, %i, %.3f) -> +(%i, %i, %.3f) => (%i, %i, %.3f)", m_pos.x, m_pos.y, m_ori, mvec.x, mvec.y, dO, p.x, p.y, o));
      }

      m_path_t = millis();

      mvec.x = d;
      mvec.y = o - m_ori;
    }

    return mvec;
  }

}

namespace tuum {

  Subsystem::TypeVar Navigator::Type;

  int motion_tick_nullfn(Vec2d mvec)
  {
    return -1;
  }

  Navigator::Navigator():
    Subsystem("Navigator"),
    m_motion_handler(motion_tick_nullfn),
    gLoc(nullptr),
    mComposer(this),
    m_init(false),
    m_running(false)
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
        Vec2i tPos(0, 0);
        float tOri = 0.0;

        if(m_ctx.flags & NAV_SET_POS) tPos = m_ctx.tPos;
        if(m_ctx.flags & NAV_SET_ORI) tOri = m_ctx.tOri;

        mPather.poseTick(pose.coord, pose.orient);
        mvec_t mvec = (mvec_t)mPather.pathTick(tPos, tOri);

        if(m_running)
          this->onMotionTick(mvec);
      }

    }
  }

  int Navigator::navTo()
  {
    m_ctx.flags &= ~(NAV_SET_POS | NAV_SET_ORI);
  }

  int Navigator::navTo(const Vec2i& pos)
  {
    m_ctx.tPos = pos;
    m_ctx.flags |= NAV_SET_POS;
    m_running = true;
  }

  int Navigator::navTo(const Vec2i& pos, const float& ori)
  {
    m_ctx.tPos = pos;
    m_ctx.tOri = ori;
    m_ctx.flags |= NAV_SET_POS | NAV_SET_ORI;
    m_running = true;
  }

  int Navigator::aim()
  {
    m_ctx.flags &= ~NAV_SET_AIM;
  }

  int Navigator::aim(const Vec2i& pos)
  {
    m_ctx.aPos = pos;
    m_ctx.flags |= NAV_SET_AIM;
    m_running = true;
  }

  void Navigator::clearGoal()
  {
    m_ctx.flags = 0;
  }

  void Navigator::stop()
  {
    m_running = false;
  }

  bool Navigator::isTargetAchieved()
  {
    return false; // TODO
  }

  int Navigator::globalToMap(const gps_t& in, Vec2i& out)
  {
    if(!mMap.isAnchorFixed()) return -1;

    gps_t gO = gps_t(mMap.getAnchor());  // Global map origin

    out = gO.metricVectorTo(in);

    return 1;
  }

  int Navigator::mapToGlobal(const Vec2i& in, gps_t& out)
  {
    if(!mMap.isAnchorFixed()) return -1;

    return 1;
  }

  int Navigator::isReachable(Vec2i p)
  {
    //TODO
    return 1;
  }

  int Navigator::feedLandmarks(LandmarkSet* lms)
  {

  }

  int Navigator::getLocalMaps(EnvMapPtrSet& out)
  {
    out.push_back(&mMap);
  }

  void Navigator::setMotionHandler(Navigator::MotionHandlerFn_t ptr)
  {
    m_motion_handler = ptr;
  }

  void Navigator::onMotionTick(mvec_t mvec)
  {
    if(m_motion_handler == nullptr) return;
    int res = m_motion_handler(mvec);

    if(res < 0) {
      RTXLOG(tuum::format("motion handler error %i", res), LOG_ERR);
    }
  }

  void Navigator::setStaticNavmeshRect(const rect_t& in)
  {
    // mMap.setStaticArea(in);
  }

}
