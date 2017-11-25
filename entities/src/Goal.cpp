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

  Goal::Goal(const Blob& blob):
    Entity(blob)
  {

  }

  Goal::Goal(const Transform& transform, const Blob& blob):
    Entity(transform, blob)
  {
    // Nothing to do here
  }

  bool Goal::isAlly() {
    std::string pattern = tuum::gC.getStr("Pattern.AllyGoal");
    /*
    if (pattern == std::string("Y"))
      return getType() == Blob::TypeId::Goal_Yellow;
    else
      return getType() == Blob::TypeId::Goal_Blue;
    */
  }

  bool Goal::isOpponent() {
    std::string pattern = tuum::gC.getStr("Pattern.OpponentGoal");
    /*
    if (pattern == std::string("Y"))
      return getType() == Blob::TypeId::Goal_Yellow;
    else
      return getType() == Blob::TypeId::Goal_Blue;
    */
  }

  std::string Goal::toString()
  {
    auto wpos = mTransform.getPosition(), lpos = mBlob.getCentroid();
    return tuum::format("<G#%lu Frame(%i, %i)/World(%i, %i) f_n=%i>", mId, lpos.x, lpos.y, wpos.x, wpos.y, m_alive_frames);
  }

}
