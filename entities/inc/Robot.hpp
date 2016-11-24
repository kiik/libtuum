/** @file Robot.hpp
 *  @brief Robot representation.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 *  @date 26. November 2015
 */

#ifndef TUUM_RTX_ROBOT_H
#define TUUM_RTX_ROBOT_H

#include "Entity.hpp"

namespace tuum {

  class Robot : public Entity
  {
  public:
    Robot();
    Robot(const Transform&, const Blob&);

    bool isAlly() const;
    bool isOpponent() const;
  };

}

#endif
