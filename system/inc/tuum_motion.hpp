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

#include "tuum_json.hpp"
#include "tuum_math.hpp"

#include "MotionControl.hpp"

#include "syscore/MotionData.hpp"

namespace tuum {
namespace hal {
    class MotionControl;
}}

namespace tuum {

  class Motion : public JSONSerializable<Motion>
  {
  public:
    Motion();

    void init();
    void run();

    void loop();

    void setPositionTarget(Vec2i&);

    void setTarget(Vec2i);
    void setTarget(double);
    void setTarget(Vec2i, double);
    void setTarget(Vec2i, Vec2i);

    void setAimTarget(Vec2i);

    void start();
    void stop();

    bool isRunning();

    double getDistanceError(); // Returns distance error from target
    double getOrientError();   // Returns angle error from target

    // Returns motion completion state.
    bool isTargetAchieved();

    bool isLocationAchieved();
    bool isOrientAchieved();

    void toJSON(json&);

    void debug();

  protected:
    bool m_running, m_target_achieved;

    MotionData mMotionData;
    Timer mMotorTmr;

    hal::MotionControl* gMotionControl;
  public:
    static int MinDist;
    static int MinOrient;
    static int DribblerPlaneOffset;
    static int DribblerPlanePadding;
  };

}

#endif
