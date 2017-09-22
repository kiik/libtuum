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

#include "platform.hpp"

using namespace tuum;

#include <cmath>


namespace tuum {

  struct gps_t {
    double lat, lon;
    time_ms_t _t;

    gps_t():
      lat(0), lon(0),
      _t(0)
    {

    }

    gps_t(double la, double ln):
      lat(la), lon(ln),
      _t(0)
    {

    }

    template<typename T>
    gps_t(const T& o):
      gps_t(o.x, o.y)
    {

    }

    Vec2d metricAddition(Vec2d deltaMeters)
    {
      const double C1 = 180/3.14, C2 = 6378137.0;
      
      Vec2d out;

      out.x = lat + C1 * (deltaMeters.y / C2);
      out.y = lon + C1 * (deltaMeters.x / C2) / cos(lat);
    
      return out;
    }

    void toMetricRect(Vec2i dim, Vec2d out[4])
    {
      Vec2d dim_2 = {dim.x / 2.0, dim.y / 2.0};
      
      out[0] = metricAddition({-dim_2.x, -dim_2.y});
      out[1] = metricAddition({dim_2.x, -dim_2.y});
      out[2] = metricAddition({dim_2.x, dim_2.y});
      out[3] = metricAddition({-dim_2.x, dim_2.y});
    }

    gps_t roundToNearest_km()
    {
      gps_t out;

      // Lat: 1 deg = 110.574 km -> 1 km - 0.009043717
      // Lng: 1 deg = 111.320 km * cos(lat) -> 1km - 0.008983112 / cos(lat)
      const double deg_to_rad = 3.14 / 180.0;
      const uint64_t P = pow(10, 12);
      const uint64_t lat_km_C = 0.009043717 * P, lon_km_C = 0.008983112 * cos(lat * deg_to_rad) * P;

      out.lat = lat - ((uint64_t)(lat * P) % lat_km_C) / (double)P;
      out.lon = lon - ((uint64_t)(lon * P) % lon_km_C) / (double)P;

      return out;
    }

    Vec2d getMetricDelta(const Vec2d& o)
    {
      
    }

    double distanceTo(const gps_t& o)
    {
      const double R = 6371; // metres
      const double deg_to_rad = PI / 180.0;

      double dLat_2 = ((o.lat - lat) * deg_to_rad) / 2.0,
             dLon_2 = ((o.lon - lon) * deg_to_rad) / 2.0;

      double a = sin(dLat_2) * sin(dLat_2) +
                 cos(lat * deg_to_rad) * cos(o.lat * deg_to_rad) *
                 sin(dLon_2) * sin(dLon_2);

      double c = 2 * atan2(sqrt(a), sqrt(1-a));

      return R * c * 1000;
    }

    double bearingTo(const gps_t& o)
    {
      const double deg_to_rad = PI / 180.0;
      
      double dLon = (o.lon - lon) * deg_to_rad,
        l1 = lat * deg_to_rad, l2 = o.lat * deg_to_rad;
      
      double y = sin(dLon) * cos(l2),
             x = cos(l1) * sin(l2) -
              sin(l1) * cos(l2) * cos(dLon);
      
      return atan2(y, x);
    }

    Vec2i metricVectorTo(const gps_t& o)
    {
      double a = bearingTo(o), d = distanceTo(o);
      return Vec2i(sin(a)*d, cos(a)*d);
    }

    /*
    double getTargetBearing(Vec2d o)
    {
      double y = Math.sin(dLon) * Math.cos(lat2);
      double x = Math.cos(lat1)*Math.sin(lat2) -
              Math.sin(lat1)*Math.cos(lat2)*Math.cos(dLon);
      var brng = Math.atan2(y, x).toDeg();
    }*/

  };

}


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
