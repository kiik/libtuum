/** @file Blob.cpp
 *  @brief Class containing statistical information about a color blob on the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.4
 *  @date 5. December 2015
 */

#ifndef TUUM_BLOB_H
#define TUUM_BLOB_H

#include <vector>

#include "tuum_math.hpp"

namespace tuum {

  class Blob
  {
  public:
    Blob();
    Blob(blob_t);

    rect_t getRect() const;

    vec2i getCentroid();
    vec2i getPosition();

    vec2i getWorldPosition();
    vec2i getWorldSize();

    //double getWorldDistance()
    //double getWorldAngle()

    double getDistance();
    double getAngle();

    void merge(Blob&);

  protected:
    rect_t mRect;
  };

  typedef std::vector<Blob> BlobSet;

}

#endif
