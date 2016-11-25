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

  int Motion::DribblerPlaneOffset = 20;
  int Motion::DribblerPlanePadding = 20;

  Motion::Motion():
    m_running(true), m_target_achieved(true)
  {
    mMotorTmr.setPeriod(100);
    mMotorTmr.start();
  }

  void Motion::setup() {
    gMotorControl = hal::hw.getMotorControl();
  }

  void Motion::process() {
    if(!m_running) return;

    // Step 1: Calculate target vectors
    mMotionData.calc();

    // Step 2: Send motor command
    if(!isTargetAchieved()) {
      if(mMotorTmr.isTime()) {
        float sp = mMotionData.getSpeed();
        float he = mMotionData.getHeading();
        float rsp = mMotionData.getRotationSpeed();
        //gMotorControl->omniDrive(sp, he, rsp);
        mMotorTmr.start();
      }
    }
  }

  void Motion::setTarget(vec2i pos) {

  }

  void Motion::setTarget(double angle) {
    //TODO
  }

  void Motion::setTarget(vec2i pos, double angle) {
    //TODO
  }

  void Motion::setTarget(vec2i pos, vec2i view_target) {
    //TODO
  }

  void Motion::setAimTarget(vec2i trg) {
    //TODO: Calculate target orientation from input vector
  }

  void Motion::start() {
    m_running = true;
  }

  void Motion::stop() {
    m_running = false;
    //TODO: Send stop motor command
  }

  bool Motion::isRunning() { return m_running; }

  double Motion::getDistanceError() {
    //TODO
    return 0.0;
  }

  double Motion::getOrientError() {
    //TODO
    return 0.0;
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

}
