/** @file Robot.cpp
 *  Robot class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 3 December 2015
 */

#include "tuum_platform.hpp"

#include "Robot.hpp"

namespace tuum {

  Robot::Robot() {

  }

  Robot::Robot(const Transform& transform, const Blob& blob):
    Entity(transform, blob)
  {
    // Nothing to do here
  }

  bool Robot::isAlly() const {
    std::string pattern = tuum::gC.getStr("Pattern.Ally");
    if (pattern == std::string("YB")) {
      return false; //blob->isYellowBlue();
    } else {
      return false; //blob->isBlueYellow();
    }
  }

  bool Robot::isOpponent() const {
    std::string pattern = tuum::gC.getStr("Pattern.Opponent");
    if (pattern == std::string("YB")) {
      return false; //blob->isYellowBlue();
    } else {
      return false; //blob->isBlueYellow();
    }
  }

}
