/** @file tuum_math.hpp
 *  @brief Consolidated mathemathical structures and functions.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 19. November 2015
 */

#ifndef TUUM_MATH_H
#define TUUM_MATH_H

#include <cmath>

#include "__future__.hpp"

#include "Vec.hpp"
#include "Transform.hpp"

#include "tuum_vector.hpp"

namespace tuum {

  struct rect_t {
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;

    rect_t()
    {
      x0 = 0; y0 = 0; x1 = 0; y1 = 0;
    }

    rect_t(int X0, int Y0, int X1, int Y1)
    {
      x0 = X0; y0 = Y0; x1 = X1; y1 = Y1;
    }

    void merge(const rect_t& o) {
      x0 = std::min(x0, o.x0);
      y0 = std::min(y0, o.y0);
      x1 = std::max(x1, o.x1);
      y1 = std::max(y1, o.y1);
    }

    size_t getArea() {
      return (y1 - y0) * (x1 - x0);
    }

    vec2i getCentroid() {
      return {(x1 - x0) / 2 + x0, (y1 - y0) / 2 + y0};
    }

  };

  struct blob_t {
    rect_t rect;
    size_t realArea;

    blob_t():
      realArea(0)
    {
    }

    size_t getArea() {
      return rect.getArea();
    }

    double getDensity() {
      return (double)realArea / getArea();
    }

    vec2i getCentroid() {
      return rect.getCentroid();
    }

  };

}

#endif // RTX_MATH_H
