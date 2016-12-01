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

  bool Goal::isAlly() {
    std::string pattern = tuum::gC.getStr("Pattern.AllyGoal");
    if (pattern == std::string("Y"))
      return getType() == Blob::TypeId::Goal_Yellow;
    else
      return getType() == Blob::TypeId::Goal_Blue;
  }

  bool Goal::isOpponent() {
    std::string pattern = tuum::gC.getStr("Pattern.OpponentGoal");
    if (pattern == std::string("Y"))
      return getType() == Blob::TypeId::Goal_Yellow;
    else
      return getType() == Blob::TypeId::Goal_Blue;
  }

  Blob::TypeId Goal::getType() {
    return mBlob.getType();
  }

}
