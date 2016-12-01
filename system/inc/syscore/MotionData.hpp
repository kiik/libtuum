/** @file MotionData.hpp
 *  @brief Motion calculations class.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 1. November 2015
 */

#ifndef TUUM_MOTION_DATA_H
#define TUUM_MOTION_DATA_H

#include <stdint.h>

#include "PID.hpp"

#include "tuum_vector.hpp"

namespace tuum {

  class MotionData
  {
  public:
    MotionData();

    void setTargetPosition(vec2i);
    void setTargetOrientation(double);

    double getDistanceDelta();
    double getOrientationDelta();

    vec2i getTarget() { return tPos; }

  protected:
    vec2i tPos;
    double tOrient;

    size_t t;
    double avg_dt;

    /**
     *  Omni drive parameter cmalculation members
     */
    double Pv, Pr; // Speed PID control constants (TODO: Implement PID controller usage)

    int16_t m_speed;
    double   m_heading;
    int16_t m_r_speed;

    PID mRotCtl;
    PID mSpdCtl;

  public:
    int16_t getSpeed() { return m_speed; }
    double getHeading() { return m_heading; }
    int16_t getRotationSpeed() { return m_r_speed; }

    double getAvgDeltaTime() { return avg_dt; }

    void calc();
    void clear();
  };

}

#endif
