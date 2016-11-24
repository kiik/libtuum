/**
 *  @file Goal.cpp
 *  Goal class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 2 December 2015
 */

#include "Goal.hpp"

#include "tuum_platform.hpp"


namespace tuum {

  Goal::Goal():
    Entity()
  {
    // Nothing to do here
  }

  Goal::Goal(const Transform& transform, const Blob& blob):
    Entity(transform, blob)
  {
    // Nothing to do here
  }

  bool Goal::isAlly() const {
    std::string pattern = tuum::gC.getStr("Pattern.AllyGoal");
    if (pattern == std::string("Y")) {
      return false; //blob->isYellow();
    } else {
      return false; //blob->isBlue();
    }
  }

  bool Goal::isOpponent() const {
    std::string pattern = tuum::gC.getStr("Pattern.OpponentGoal");
    if (pattern == std::string("Y")) {
      return false; //blob->isYellow();
    } else {
      return false; //blob->isBlue();
    }
  }

}
