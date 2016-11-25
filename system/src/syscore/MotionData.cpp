/** @file MotionData.cpp
 *  @brief Motion calculations class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 1. November 2015
 */

#include "syscore/MotionData.hpp"

#include "tuum_localization.hpp"

namespace tuum {

  MotionData::MotionData():
    tPos({0, 0}), tOrient(0.0),
    m_pos_set(false), m_aim_set(false)
  {

  }

  void MotionData::setTargetPosition(vec2i in) {
    tPos = in;
    m_pos_set = true;
  }

  void MotionData::setTargetOrientation(double in) {
    tOrient = in;
    m_aim_set = true;
  }

  double MotionData::getDistanceDelta() {
    //TODO
    return 0.0;
  }

  double MotionData::getOrientationDelta() {
    //TODO
    return 0.0;
  }

  /**
   *  Omni drive parameter calculation members
   */
  void MotionData::calc() {
    //TODO: Calculate speeds using PID with target position error as input
    m_speed = m_pos_set ? 0 : 0;
    m_r_speed = m_aim_set ? 0 : 0;

    m_heading = tPos.getOrientation();
  }

}
