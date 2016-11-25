/** @file Point2D.hpp
 *  @brief 2-dimensional point class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 */

#ifndef TUUM_POINT2D_H
#define TUUM_POINT2D_H

#include <cmath>

using namespace std;

namespace tuum {

  //TODO: Refactor into generic vector structure
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

    template<typename T2>
    Point2D(const Point2D<T2>& o):
      x((T)o.getX()),
      y((T)o.getY())
    {

    }

    Point2D(const T& X, const T& Y):
      x(X),
      y(Y)
    {

    }

    void setX(const T& X) { x = X; }
    void setY(const T& Y) { y = Y; }

    void rotate(double alpha) {
      x = x * cos(alpha) + y * -sin(alpha);
      y = x * sin(alpha) + y * cos(alpha);
    }

    void normalizeTo(double value) {
      double s = abs(x) + abs(y);
      if(s == 0) return;
      x /= s;
      y /= s;
    }

    double getMagnitude() {
      if(x == 0.0 && y == 0.0) return 0.0;
      return sqrt(pow(x, 2) + pow(y, 2));
    }

    template<typename T2>
    double distanceTo(Point2D<T2> t) {
      Point2D<T> vec = {t.x - x, t.y - y};
      return vec.getMagnitude();
    }

    double getOrientation() {
      return atan2(y, x);
    }

    Point2D<double> getNormalized() {
      double s = abs(x) + abs(y);
      if(s == 0) return Point2D<double>({0.0, 0.0});
      return Point2D<double>({(double)x / s, (double)y / s});
    }

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

    template<typename T2>
    Point2D<T> operator+(const Point2D<T2>& o) {
      return Point2D<T>({x + o.x, y + o.y});
    }

    template<typename T2>
    Point2D<T> operator-(const Point2D<T2>& o) {
      return Point2D<T>({x - o.x, y - o.y});
    }

    template<typename T2>
    Point2D<T> operator*(T2 v) {
      x *= v; y *= v;
    }

  };

};

#endif
