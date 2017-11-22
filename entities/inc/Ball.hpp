/** @file Ball.hpp
 *  @brief Ball representation.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.3
 *  @date 2. December 2015
 */

#ifndef TUUM_RTX_BALL_H
#define TUUM_RTX_BALL_H

#include "Entity.hpp"

namespace tuum {

  class Ball: public Entity
  {
  public:
    Ball(const Blob&);
    Ball(const Transform&, const Blob&);

    bool isValid() const;

    bool isKicked() const { return m_kicked; };
    bool isInGoal() const { return m_in_goal; };
    bool isInBounds() const { return m_in_bounds; };

    void setKicked(const bool& v = true) { m_kicked = v; }
    void setInGoal(const bool& v = true) { m_in_goal = v; }
    void setInBounds(const bool& v = true) { m_in_bounds = v; }

    std::string toString();

  protected:
    bool m_kicked, m_in_goal, m_in_bounds;
  };

}

#endif
