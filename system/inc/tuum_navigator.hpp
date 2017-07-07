/** @file tuum_navigator.hpp
 *  @brief Navigator system processes perception data,
 *  generates navmesh & pathing data, handles high-level motion commands.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 2. June 2017
 */

#ifndef TUUM_NAVIGATION_H
#define TUUM_NAVIGATION_H

#include "loc_landmark_stream.hpp"

#include "tuum_system.hpp"

using namespace loc;

namespace tuum {

  class Localizer;

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

    Vec2d getAnchor() { return m_gps_anchor; }

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

    Subsystem::Type getType() { return Navigator::GetType(); }

    enum NavigatorFlagsE {
      NAV_SET_POS = 0x01,
      NAV_SET_ORI = 0x02,
    };

    struct ctx_t {
      Vec2i tPos;
      float tOri;

      uint8_t flags = 0;

      bool hasTarget() { return (flags & NAV_SET_POS) == 1; }
      bool hasOrient() { return (flags & NAV_SET_ORI) == 1; }
    };

  public:
    Navigator();

    OctreeLandmarkMap* getMap() { return &mMap; }

    int init();

    void setup();
    void process();

    int navTo(const Vec2i&);
    int navTo(const Vec2i&, const float&);
    // int navTo(const float&);

    ctx_t getContext() { return m_ctx; }

    int feedLandmarks(LandmarkSet*);

    int getLocalMaps(EnvMapPtrSet&);

  protected:
    ctx_t m_ctx;

    Localizer *gLoc;

    bool m_init;

    OctreeLandmarkMap mMap;
  };

}

#endif
