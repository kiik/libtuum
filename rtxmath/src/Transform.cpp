/** @file Transform.cpp
 *  Object positional transform state class.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 19. November 2015
 */

#include <string>
#include <sstream>

#include "Transform.hpp"

namespace tuum {

  Transform::Transform() {
    pos.x = 0; pos.y = 0;
    o = 0;
  }

  Transform::Transform(const int x, const int y) {
    pos.x = x; pos.y = y;
  }

  Transform::Transform(const vec2i vec) {
    pos = vec;
  }

  Transform::Transform(const int x, const int y, const double orient) {
    pos.x = x; pos.y = y;
    o = orient;
  }

  Transform::Transform(const vec2i vec, const double orient) {
    pos = vec;
    o = orient;
  }

  void Transform::setPosition(const int x, const int y) {
    pos.x = x; pos.y = y;
  }

  void Transform::setPosition(const vec2i nPos) {
    pos = nPos;
  }

  void Transform::setOrientation(const double orient) {
    o = orient;
  }

  vec2i Transform::getPosition() {
    return pos;
  }

  double Transform::getOrientation() {
    return o;
  }

  int Transform::getX() {
    return pos.x;
  }

  int Transform::getY() {
    return pos.y;
  }

  void Transform::setX(int v) {
    pos.x = v;
  }

  void Transform::setY(int v) {
    pos.y = v;
  }

  double Transform::distanceTo(vec2i target) {
    return sqrt(pow(target.x - pos.x, 2) + pow(target.y - pos.y, 2));
  }

  Transform Transform::operator-(Transform t) {
    return Transform({{pos.x - t.getX(), pos.y - t.getY()}, o - t.o});
  }

  std::string Transform::toString() {
    std::stringstream output;
    output << "<Transform "
           << "(" << pos.x
	   << ", " << pos.y
	   << ", " << o
	   << ")>";
    return output.str();
  }

}
