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

  Blob::Blob(std::string name, blob_t blob):
    m_name(name), mRect(blob.rect),
    m_area(blob.realArea)
  {
    // Nothing to do here
  }

  rect_t Blob::getRect() const { return mRect; }

  Vec2i Blob::getCentroid() {
    return mRect.getCentroid();
  }

  Vec2i Blob::getPosition() {
    Vec2i c = mRect.getCentroid();
    return Vec2i(c.x, c.y);
  }

  Vec2i Blob::getWorldPosition() {
    Vec2i p = getPosition();
    return Vision::Perspective::imageToWorld(p);
  }

  double Blob::getDensity() {
    return (double)m_area / mRect.getArea();
  }

  double Blob::getDistance() {
    Vec2i p = getWorldPosition();
    return sqrt(p.x * p.x + p.y * p.y);
  }

  double Blob::getAngle() {
    Vec2i p = getWorldPosition(); //TODO: Take into account our position
    return atan2(p.y, p.x);
  }

  void Blob::merge(Blob &o) {
    mRect.merge(o.getRect());
  }

}
