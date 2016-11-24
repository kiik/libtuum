/** @file Blob.cpp
 *  @brief Class containing statistical information about a color blob on the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.4
 *  @date 5. December 2015
 */

#include <cmath>

#include "Perspective.hpp"

#include "Blob.hpp"

namespace tuum {

  Blob::Blob()
  {

  }

  Blob::Blob(blob_t primitive):
    mRect(primitive.rect)
  {
    // Nothing to do here
  }

  rect_t Blob::getRect() const { return mRect; }

  vec2i Blob::getCentroid() {
    return mRect.getCentroid();
  }

  vec2i Blob::getPosition() {
    vec2i c = mRect.getCentroid();
    return vec2i(c.x, c.y);
  }

  vec2i Blob::getWorldPosition() {
    vec2i p = getPosition();
    return vec2i(0, 0); //Vision::Perspective::virtualToReal(position, cameraID);
  }

  double Blob::getDistance() {
    vec2i p = getWorldPosition();
    return sqrt(p.x * p.x + p.y * p.y);
  }

  double Blob::getAngle() {
    vec2i p = getWorldPosition(); //TODO: Take into account our position
    return -atan2(p.x, p.y);
  }

  void Blob::merge(Blob &o) {
    mRect.merge(o.getRect());
  }

}
