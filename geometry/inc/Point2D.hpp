/** @file Point2D.hpp
 *  @brief 2-dimensional point class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 */

#ifndef TUUM_POINT2D_H
#define TUUM_POINT2D_H

#include <cmath>

namespace tuum {

  template<typename T = double>
  struct Point2D
  {
    T x;
    T y;

    Point2D()
    {
      x = 0;
      y = 0;
    }

    Point2D(const Point2D<T>& o):
      x(o.getX()),
      y(o.getY())
    {

    }

    Point2D(const T& X, const T& Y):
      x(X),
      y(Y)
    {

    }

    void setX(const T& X) { x = X; }
    void setY(const T& Y) { y = Y; }

    template<typename T2 = double>
    T2 distanceFrom(const Point2D<T>* o) {
      return sqrt(x * o->getX() + y * o->getY());
    }

    template<typename T2 = double>
    T2 distanceFrom(const Point2D<T>& o) {
      return sqrt(x * o.getX() + y * o.getY());
    }

    T getX() const { return x; };
    T getY() const { return y; };
  };

};

#endif
