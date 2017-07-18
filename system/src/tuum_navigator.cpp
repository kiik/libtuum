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

using namespace tuum::hal;

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
    out["anchor"] = {m_anchor.x, m_anchor.y};
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
    Vec2i mvec(0, 0);
    
    if(t - m_pose_t <= 250) // 4Hz minimum pose update frequency
    {
      //TODO: implement pathfinding with intermediate waypoints

      mvec.x = tP.x - m_pos.x;
      mvec.y = tP.y - m_pos.y;

      m_path_t = millis();
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
        Vec2i tPos(0, 0);
        float tOri = 0.0;

        if(m_ctx.flags & NAV_SET_POS) tPos = m_ctx.tPos;
        if(m_ctx.flags & NAV_SET_ORI) tOri = m_ctx.tOri;

        mPather.poseTick(pose.coord, pose.orient);
        mvec_t mvec = (mvec_t)mPather.pathTick(tPos, tOri);

        this->onMotionTick(mvec);
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

  int Navigator::globalToMap(const Vec2d& in, Vec2i out)
  {
    if(!mMap.isAnchorFixed()) return -1;

    gps_t gO = gps_t(mMap.getAnchor());  // Global map origin

    out = (Vec2i)(gO.getMetricDelta(in));

    return 1;
  }

  int Navigator::mapToGlobal(const Vec2i& in, Vec2d out)
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

}
