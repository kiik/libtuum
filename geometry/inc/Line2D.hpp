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
    Line2D(Vec2f*, Vec2f*);
    Line2D(const double&, const double&, const double&, const double&);

    void setPointA(const Vec2f*);
    void setPointA(const double&, const double&);
    void setPointB(const Vec2f*);
    void setPointB(const double&, const double&);

    double getLength();
    Vec2f* getPointA();
    Vec2f* getPointB();

  private:
    Vec2f *pointA;
    Vec2f *pointB;
  };

};

#endif
