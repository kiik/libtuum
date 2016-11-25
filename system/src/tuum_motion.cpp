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

  Motion::MinDist = GRS_MOV.low.step;
  Motion::MinOrient = GRS_ROT.low.step;

  Motion::Motion():
    m_running(true), m_target_achieved(true)
  {
    motorCmdTimer.setPeriod(100);
    motorCmdTimer.start();
  }

  void Motion::setup() {
    gMotorControl = hal::hw.getMotorControl();
  }

  void Motion::process() {
    if(!m_running) return;

    // Step 1: Calculate target vectors
    //motionData.calc();
    //motionData.applyFactors();
    //motionData.clamp();

    // Step 2: Send motor command
    if(!isTargetAchieved()) {
      if(mMotorTmr.isTime()) {
        //mot.sp = motionData.getSpeed();
        //mot.tr = motionData.getHeading();
        //mot.jo = motionData.getRotationSpeed();
        //mco->move(mot);
        motorCmdTimer.start();
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

  bool isTargetAchieved() {
    return isLocationAchieved() && isOrientAchieved();
  }

  bool isLocationAchieved() {
    return getDistanceError() <= Motion::MinDist;
  }

  bool isOrientAchieved() {
    return getOrientError() <= Motion::MinOrient;
  }

}}
