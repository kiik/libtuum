/** @file tuum_motion.hpp
 *  @brief Motion managment system.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 1. November 2015
 */

#ifndef TUUM_MOTIONING_H
#define TUUM_MOTIONING_H


#include "platform.hpp"
#include "tuum_platform.hpp"
#include "tuum_math.hpp"

#include "hal.hpp"

#include "syscore/MotionData.hpp"

namespace tuum {

  class Motion
  {
  public:
    Motion();

    void setup();
    void process();

    void setPositionTarget(vec2i&);

    void setTarget(vec2i);
    void setTarget(double);
    void setTarget(vec2i, double);
    void setTarget(vec2i, vec2i);

    void setAimTarget(vec2i);

    void start();
    void stop();

    bool isRunning();

    double getDistanceError(); // Returns distance error from target
    double getOrientError();   // Returns angle error from target

    // Returns motion completion state.
    bool isTargetAchieved();

    bool isLocationAchieved();
    bool isOrientAchieved();

  protected:
    bool m_running, m_target_achieved;

    MotionData mMotionData;
    Timer mMotorTmr;

    hal::MotorControl* gMotorControl;
  public:
    static int MinDist;
    static int MinOrient;
    static int DribblerPlaneOffset;
    static int DribblerPlanePadding;
  };

}

#endif
