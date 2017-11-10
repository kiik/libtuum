/**
 * @file Line.cpp
 * Line class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Line2D.hpp"


namespace tuum {

  Line2D::Line2D(const Line2D &other):
    pointA{new Vec2f(other.getPointA()->x, other.getPointB()->y)},
    pointB{new Vec2f(other.getPointB()->x, other.getPointB()->y)}
  {

  }

  Line2D::Line2D(Vec2f *pointA, Vec2f *pointB):
    pointA{new Vec2f(pointA->x, pointA->y)},
    pointB{new Vec2f(pointB->x, pointB->y)}
  {

  }

  Line2D::Line2D(const double &pointAx, const double &pointAy,
             const double &pointBx, const double &pointBy):
    pointA{new Vec2f(pointAx, pointAy)},
    pointB{new Vec2f(pointBx, pointBy)}
  {

  }

  void Line2D::setPointA(const Vec2f *pointA) {
    this->pointA = new Vec2f(pointA->x, pointA->y);
  }

  void Line2D::setPointA(const double &x, const double &y) {
    pointA = new Vec2f(x, y);
  }

  void Line2D::setPointB(const Vec2f *pointB) {
    this->pointB = new Vec2f(pointB->x, pointB->y);
  }

  void Line2D::setPointB(const double &x, const double &y) {
    pointB = new Vec2f(x, y);
  }

  double Line2D::getLength() {
    return pointA->distanceTo(pointB);
  }

  Vec2f* Line2D::getPointA() {
    return pointA;
  }

  Vec2f* Line2D::getPointB() {
    return pointB;
  }

};
