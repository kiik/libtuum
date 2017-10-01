/** @file tuum_motion.cpp
 *  @brief Motion managment system implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 1. November 2015
 */

#include "hal.hpp"

#include "tuum_localization.hpp"
#include "tuum_motion.hpp"

namespace tuum {

  //TODO: Test & reconfigure these constants
  int Motion::MinDist = 10;
  int Motion::MinOrient = 0.08;

  int Motion::DribblerPlaneOffset = 5;  // Used to stop in front of ball
  int Motion::DribblerPlanePadding = 20; // Used for blindy going forward to pickup ball

  Motion::Motion():
    m_running(true), m_target_achieved(true)
  {
    mMotorTmr.setPeriod(100);
    mMotorTmr.start();
  }

  void Motion::init() {
    gMotionControl = hal::hw.getMotionControl();
  }

  void Motion::run() {
    if(!m_running) return;

    // Step 1: Calculate target vectors
    mMotionData.calc();

    // Step 2: Send motor command
    if(!isTargetAchieved()) {
      if(mMotorTmr.isTime()) {
        float sp = mMotionData.getSpeed();
        float he = mMotionData.getHeading();
        float rsp = mMotionData.getRotationSpeed();
        gMotionControl->omniDrive(sp, he, rsp);
        mMotorTmr.start();
      }
    } else {
      stop();
      m_running = false;
    }
  }

  void Motion::loop() {
    while(1) {
      run();
    }
  }

  void Motion::setTarget(vec2i pos) {
    setTarget(pos, 0.0);
  }

  void Motion::setTarget(double angle) {
    setTarget({0, 0}, angle);
  }

  void Motion::setTarget(vec2i pos, double angle) {
    mMotionData.clear();
    mMotionData.setTargetPosition(pos);
    mMotionData.setTargetOrientation(angle);
    start();
  }

  void Motion::setTarget(vec2i pos, vec2i view_target) {
    mMotionData.clear();
    mMotionData.setTargetPosition(pos);


    //TODO: Take into account localization
    mMotionData.setTargetOrientation(view_target.getNormalized().getOrientation());

    start();
  }

  void Motion::setAimTarget(vec2i trg) {
    setTarget(trg.getOrientation());
  }

  void Motion::start() {
    m_running = true;
  }

  void Motion::stop() {
    m_running = false;
    setTarget({0, 0}, 0.0);
    gMotionControl->omniDrive(0, 0, 0);
  }

  bool Motion::isRunning() { return m_running; }

  double Motion::getDistanceError() {
    return mMotionData.getDistanceDelta();
  }

  double Motion::getOrientError() {
    return mMotionData.getOrientationDelta();
  }

  bool Motion::isTargetAchieved() {
    return isLocationAchieved() && isOrientAchieved();
  }

  bool Motion::isLocationAchieved() {
    return getDistanceError() <= Motion::MinDist;
  }

  bool Motion::isOrientAchieved() {
    return getOrientError() <= Motion::MinOrient;
  }

  void Motion::toJSON(json& out) {
    json dat = json::object();

    dat["distanceError"] = getDistanceError();
    dat["orientError"] = getOrientError();

    vec2i t = mMotionData.getTarget();
    dat["target"] = {t.x, t.y};

    out["motion"] = dat;
  }

  void Motion::debug() {
    printf("[Motion]: s=%.2f, dO=%.2f\n", getDistanceError(), getOrientError());
    vec2 t = mMotionData.getTarget();
    float sp = mMotionData.getSpeed();
    float he = mMotionData.getHeading();
    float rsp = mMotionData.getRotationSpeed();

    printf("[Motion]: sp=%.2f, h2=%.2f, rsp=%.2f. (dt=%.2f)\n", sp, he, rsp, mMotionData.getAvgDeltaTime());
  }

}
