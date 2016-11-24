/** @file Line.hpp
 *  @brief 2D line representation.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 */

#ifndef TUUM_LINE2D_H
#define TUUM_LINE2D_H

#include "tuum_vector.hpp"

namespace tuum {

  class Line2D
  {
  public:
    Line2D(const Line2D&);
    Line2D(const vec2*, const vec2*);
    Line2D(const double&, const double&, const double&, const double&);

    void setPointA(const vec2*);
    void setPointA(const double&, const double&);
    void setPointB(const vec2*);
    void setPointB(const double&, const double&);

    double getLength() const;
    vec2* getPointA() const;
    vec2* getPointB() const;

  private:
    vec2 *pointA;
    vec2 *pointB;
  };

};

#endif
