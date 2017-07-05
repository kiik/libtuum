/** @file tuum_localizer.cpp
 *  @brief Calculates local pose & manages local-to-global transformations.
 *
 *  @author Meelik Kiik (kiik.meelik@gmail.com)
 *  @version 0.1
 *  @date 4. June 2017
 */

#include <cmath>

#include "tuum_hal.hpp"

#include "sensors/GPS.hpp"

#include "tuum_navigator.hpp"
#include "tuum_localizer.hpp"

namespace tuum {

  Subsystem::TypeVar Localizer::Type;

  Localizer::Localizer():
    Subsystem("Localizer"),
    m_init(false),
    m_gps_t(0)
  {
    m_pose.coord.x = 0;
    m_pose.coord.y = 0;
    m_pose.orient = 0.0;
    m_pose.map_id = 0;
  }

  Vec2i Localizer::toAbsolute(Vec2i v) {
    // #TODO: return v - mRobotPos;
    return v - Vec2i({200, 200});
  }

  int Localizer::updateGlobalPosition()
  {
    if(millis() - m_gps_t < 1000) return 0; // Is limiting neccesary?

    auto ptr = hal::hw.getSensors();
    hal::GPSData dat = ptr->getGPS();

    m_gps.x = dat.lat;
    m_gps.y = dat.lng;
    m_gps_t = dat._t;

    if(millis() - m_gps_t > 5000) return 0;

    return 1;
  }

  void Localizer::setup()
  {
    int err_flag = 0;

    if(getSubsystemHandle<tuum::Navigator*>(Navigator::GetType(), gNavi) > 0) {
      RTXLOG("Navigator present.");
    } else err_flag = -1;

    if(err_flag < 0) {
      RTXLOG(tuum::format("Error - Missing module. (code=%i)", err_flag), LOG_ERR);
    } else {
      m_init = true;
    }
  }

  void Localizer::process()
  {
    if(updateGlobalPosition() > 0) {
      auto ptr = gNavi->getMap();
      Vec2d anchor = gps_round_to_km(m_gps);

      if(!ptr->isAnchorFixed()) {
        printf("anchor: (%.12f, %.12f) -> (%.12f, %.12f)\n", m_gps.x, m_gps.y, anchor.x, anchor.y);
        ptr->updateAnchor(anchor);
      } else {
        //TODO: Map chunking
        anchor = ptr->getAnchor();
      }

      auto imu = hal::hw.getSensors()->getIMU();

      m_pose.map_id = gNavi->getMap()->getId();
      m_pose.coord = (Vec2i)gps_calc_diff_vec(anchor, m_gps);
      m_pose.orient = imu.head;
      m_pose._t = millis();
    }
  }

  int Localizer::getLocalPose(localized_pose_t& out)
  {
    if(millis() - m_pose._t >= 4000) return -1;
    out = m_pose;
    return 0;
  }

  int Localizer::getGlobalPosition(Vec2d& out) {
    int res = updateGlobalPosition();
    out = m_gps;
    return res;
  }




}
