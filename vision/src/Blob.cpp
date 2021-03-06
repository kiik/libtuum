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

  Blob::Blob(blob_t blob):
    mRect(blob.rect)
  {
    // Nothing to do here
  }

  Blob::Blob(blob_t blob, Blob::TypeId type):
    Blob(blob)
  {
    m_real_area = blob.realArea;
    mEntType = type;
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
    return Vision::Perspective::imageToWorld(p);
  }

  double Blob::getDensity() {
    return (double)m_real_area / mRect.getArea();
  }

  double Blob::getDistance() {
    vec2i p = getWorldPosition();
    return sqrt(p.x * p.x + p.y * p.y);
  }

  double Blob::getAngle() {
    vec2i p = getWorldPosition(); //TODO: Take into account our position
    return atan2(p.y, p.x);
  }

  void Blob::merge(Blob &o) {
    mRect.merge(o.getRect());
  }

}
