/** @file MotionData.cpp
 *  @brief Motion calculations class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 1. November 2015
 */

#include <algorithm>

#include "PID.hpp"

#include "syscore/MotionData.hpp"

#include "tuum_localization.hpp"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

namespace tuum {

  PID mRotCtl;

  MotionData::MotionData():
    tPos({0, 0}), tOrient(0.0)
  {
    Pv = 1.0;
    Pr = 180.0 / 3.14;
  }

  void MotionData::setTargetPosition(vec2i in) {
    tPos = in;
  }

  void MotionData::setTargetOrientation(double in) {
    tOrient = in;
  }

  double MotionData::getDistanceDelta() {
    return tPos.getMagnitude();
  }

  double MotionData::getOrientationDelta() {
    return tOrient;
  }

  /**
   *  Omni drive parameter calculation members
   */
  void MotionData::calc() {
    //TODO: Calculate speeds using PID with target position error as input
    size_t t1 = millis();
    double dt = (t1 - t) / 1000.0;

    avg_dt = avg_dt * 0.2 + dt * 0.8;

    m_speed = Pv * tPos.getMagnitude();
    m_heading = tPos.getOrientation();
    m_r_speed = mRotCtl.run(Pr * tOrient, dt);

    if(abs(m_speed) > 40)
      m_speed = 40 * m_speed / abs(m_speed);

    if(abs(m_r_speed) > 170)
      m_r_speed = 170 * m_r_speed / abs(m_r_speed);

    t = t1;
  }

  void MotionData::clear() {
    tPos = {0, 0};
    tOrient = 0;
  }

}
