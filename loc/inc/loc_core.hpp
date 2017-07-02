/** @file loc_core.hpp
 *  Core localization functions.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 24. October 2015
 */

#ifndef LOC_CORE_H
#define LOC_CORE_H

#include <cstdlib>
#include <vector>
#include <string>

#include "rtxmath.hpp"
#include "Vec.hpp"

using namespace tuum;

namespace loc {

  struct SystemState {
    int x; int y; double orientation;

    std::string serialize() {
      std::string output = "<SystemState ";
      output += std::to_string(x);
      output += ", ";
      output += std::to_string(y);
      output += ", ";
      output += std::to_string(orientation);
      output += ">";
      return output;
    }
  };

  double random_uniform(double mn, double mx);
  double random_uniform(int mn, int mx);

  template<typename T>
  class EuclideanEntity2D
  {
  public:
    typedef tuum::Vec2D<T> transform_t;

    EuclideanEntity2D() {}
    EuclideanEntity2D(T x, T y) { m_transform.x = x; m_transform.y = y; }

    int getX() { return m_transform.x; }
    int getY() { return m_transform.y; }

    transform_t* getTransform() { return &m_transform; }

    double getDistance() { return m_distanceEstimate; }
    void setDistanceEstimate(double v) { m_distanceEstimate = v; }

    double distanceTo(T x, T y) {
      return sqrt(pow(m_transform.x - x, 2) + pow(m_transform.y - y, 2));
    }

  protected:
    transform_t m_transform;
    double m_distanceEstimate;
  };

  class Landmark : public EuclideanEntity2D<int>
  {
  public:
    Landmark(int, int);

    int consumeMeasurement(Vec2i);
  };

  typedef std::vector<Landmark> LandmarkSet;
};

#endif // RTX_LOC_CORE_H
