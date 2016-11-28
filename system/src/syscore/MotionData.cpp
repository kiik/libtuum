/** @file MotionData.cpp
 *  @brief Motion calculations class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 1. November 2015
 */

#include <algorithm>

#include "syscore/MotionData.hpp"

#include "tuum_localization.hpp"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

namespace tuum {

  MotionData::MotionData():
    tPos({0, 0}), tOrient(0.0)
  {
    Pv = 0.1;
    Pr = 0.1;
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
    m_speed = Pv * tPos.getMagnitude();
    m_heading = tPos.getOrientation();
    m_r_speed = Pr * tOrient;

    m_speed = MIN(m_speed, 40);
    m_r_speed = MIN(m_r_speed, 40);

    printf("m_speed = %d\n", m_speed );
    printf("m_r_speed = %d\n", m_r_speed);

  }

  void MotionData::clear(){
    tPos = {0, 0};
    tOrient = 0;
  }

}
