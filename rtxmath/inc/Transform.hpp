/** @file Transform.cpp
 *  Object positionsl Yransform state class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 19. November 2015
 */

#ifndef RTX_TRANSFORM_H
#define RTX_TRANSFORM_H

#include <string>
#include <math.h>

#include "tuum_vector.hpp"

namespace tuum {

  struct Transform {
    vec2i pos;
    double o;

    Transform();

    Transform(const int, const int);
    Transform(const vec2i);

    Transform(const int, const int, const double);
    Transform(const vec2i, const double);

    void setPosition(const int, const int);
    void setPosition(const vec2i);
    void setOrientation(const double);

    vec2i getPosition();
    double getOrientation();

    int getX();
    int getY();
    void setX(int);
    void setY(int);

    double distanceTo(vec2i);

    vec2i operator-(int);
    Transform operator-(Transform);

    std::string toString();
  };

}

#endif // RTX_TRANSFORM_H
