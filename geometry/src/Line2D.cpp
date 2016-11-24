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
    pointA{new vec2(other.getPointA()->getX(), other.getPointB()->getY())},
    pointB{new vec2(other.getPointB()->getX(), other.getPointB()->getY())}
  {

  }

  Line2D::Line2D(const vec2 *pointA, const vec2 *pointB):
    pointA{new vec2(pointA->getX(), pointA->getY())},
    pointB{new vec2(pointB->getX(), pointB->getY())}
  {

  }

  Line2D::Line2D(const double &pointAx, const double &pointAy,
             const double &pointBx, const double &pointBy):
    pointA{new vec2(pointAx, pointAy)},
    pointB{new vec2(pointBx, pointBy)}
  {

  }

  void Line2D::setPointA(const vec2 *pointA) {
    this->pointA = new vec2(pointA->getX(), pointA->getY());
  }

  void Line2D::setPointA(const double &x, const double &y) {
    pointA = new vec2(x, y);
  }

  void Line2D::setPointB(const vec2 *pointB) {
    this->pointB = new vec2(pointB->getX(), pointB->getY());
  }

  void Line2D::setPointB(const double &x, const double &y) {
    pointB = new vec2(x, y);
  }

  double Line2D::getLength() const {
    return pointA->distanceFrom(pointB);
  }

  vec2* Line2D::getPointA() const {
    return pointA;
  }

  vec2* Line2D::getPointB() const {
    return pointB;
  }

};
