/** @file Ball.hpp
 *  @brief Ball representation.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.3
 *  @date 2. December 2015
 */

#include "Ball.hpp"

namespace tuum {

  Ball::Ball(const Blob& blob):
    Entity(blob),
    m_kicked(false), m_in_goal(false), m_in_bounds(true)
  {

  }

  Ball::Ball(const Transform& transform, const Blob& blob):
    Entity(transform, blob),
    m_kicked(false), m_in_goal(false), m_in_bounds(true)
  {

  }

  bool Ball::isValid() const {
    return isInBounds() && !isInGoal() && !isKicked();
  }

}
