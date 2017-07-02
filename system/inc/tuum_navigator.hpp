/** @file tuum_navigator.hpp
 *  Navigator system interface. Inserts landmarks from perception subsystem
 *  into map & generates additional navmesh data for path calculation.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 2 July 2017
 */

#ifndef TUUM_NAVIGATION_H
#define TUUM_NAVIGATION_H

#include "loc_landmark_stream.hpp"

#include "tuum_system.hpp"

using namespace loc;

namespace tuum {

  struct map_octree_t {
    int x = 0, y = 0, w = 1000, h = 1000;
    std::vector<Landmark*> lms;
    map_octree_t* subtrees[4] = {nullptr,nullptr,nullptr,nullptr};
  };

  class EnvironmentMap
  {
  public:
    EnvironmentMap() {}

    virtual json toJSON() = 0;
  };

  typedef std::vector<EnvironmentMap*> EnvMapPtrSet;

  class OctreeLandmarkMap : public EnvironmentMap
  {
  public:
    static int id_seq;

  public:
    OctreeLandmarkMap();

    int init();

    int getId() { return m_id; }

    bool isAnchorFixed();
    void updateAnchor(Vec2d);

    json toJSON();

  protected:
    int m_id, m_lm_n;

    map_octree_t m_root;

    Vec2d m_gps_anchor;
    Vec2d m_gps_rect[4];
    bool m_gps_anchor_fixed;
  };

  class Navigator : public Subsystem
  {
  public:
    static Subsystem::TypeVar Type;
    static Subsystem::Type GetType() { return &Navigator::Type; }

  public:
    Navigator();

    OctreeLandmarkMap* getMap() { return &mMap; }

    int init();

    void setup();
    void process();

    Subsystem::Type getType() { return Navigator::GetType(); }

    int feedLandmarks(LandmarkSet*);

    int getLocalMaps(EnvMapPtrSet&);

  protected:
    OctreeLandmarkMap mMap;
  };

}

#endif
