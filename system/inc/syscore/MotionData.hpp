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


    /**
     *  Omni drive parameter cmalculation members
     */
    double Pv, Pr; // Speed PID control constants (TODO: Implement PID controller usage)

    uint16_t m_speed;
    double   m_heading;
    uint16_t m_r_speed;

  public:
    uint16_t getSpeed() { return m_speed; }
    double getHeading() { return m_heading; }
    uint16_t getRotationSpeed() { return m_r_speed; }

    void calc();
    void clear();
  };

}

#endif
