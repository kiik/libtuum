/** @file Goal.hpp
 *  @brief Goal representation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.3
 *  @date 2. December 2015
 */

#ifndef TUUM_RTX_GOAL_H
#define TUUM_RTX_GOAL_H

#include "Entity.hpp"

namespace tuum {

  class Goal: public Entity
  {
  public:
    Goal();
    Goal(const Transform&, const Blob&);

    bool isAlly() const;
    bool isOpponent() const;
  };

}

#endif
