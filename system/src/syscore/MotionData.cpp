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

  MotionData::MotionData():
    tPos({0, 0}), tOrient(0.0)
  {
    Pv = 0.050;
    Pr = 180.0 / 3.14;

    mRotCtl.setPID(0.8, 0.05, 0.10);
    mRotCtl.setILimit(0.5);
    
    //mRotCtl.setPID(0.1, 0.0, 0.0);

    
    mSpdCtl.setPID(0.0, 0.0, 0.0);
    mSpdCtl.setILimit(100);
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

    double d = tPos.getMagnitude();
    
    double C;

    /*m_speed = mRotCtl.run(distance, dt);
    m_heading = tPos.getOrientation();
    m_r_speed = mRotCtl.run(Pr * tOrient, dt);*/

    t = t1;
    const float NEAR_DIST = 400;
    const float SPEED = 60.0;

    if(abs(d) > NEAR_DIST)
      m_speed = SPEED;
    else
      m_speed = d * SPEED / NEAR_DIST;

    double dO = tPos.getOrientation();
    if(abs(d) > NEAR_DIST) {
      if(abs(dO) > 0.35) {
        m_heading = dO * 0.9;
        m_speed *= 0.8;
      } 
      else
        m_heading = 0.0;
    }

    if((abs(tOrient) > 0.10) && (d < 600)) {
      //m_r_speed = tOrient * 180.0 / 3.14 * d / 600 * 0.8;
      float v = (d > 200 && abs(tOrient) > 1.0) ? 1.0 : d / 100.0 * 0.5;
      m_r_speed = mRotCtl.run(tOrient, dt) * Pr * v;
    }
    else m_r_speed = 0;


    t = t1;
    //m_speed = 0;
    //m_heading = 0;
    m_r_speed = 0;
    //mRotCtl.debug(); 
  }

  void MotionData::clear() {
    tPos = {0, 0};
    tOrient = 0;
  }

}
