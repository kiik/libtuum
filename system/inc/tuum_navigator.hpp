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

#include "tuum_buff.hpp"
#include "tuum_system.hpp"

using namespace loc;

namespace tuum {

  class Localizer;
  class Navigator;

  struct map_octree_t {
    int x = 0, y = 0, w = 1000, h = 1000;
    std::vector<Landmark*> lms;
    map_octree_t* subtrees[4] = {nullptr,nullptr,nullptr,nullptr};
  };

  class EnvironmentMap
  {
  public:
    EnvironmentMap():
      m_anchor_fixed(false),
      m_anchor({0, 0})
    {

    }

    void  setAnchor(Vec2d v) { m_anchor = v; m_anchor_fixed = true; }
    Vec2d getAnchor() { return m_anchor; }
    bool isAnchorFixed() { return m_anchor_fixed; }

    Vec2i globalToMap(const Vec2d&);
    Vec2d mapToGlobal(const Vec2i&);

    virtual json toJSON() = 0;

  protected:
    bool m_anchor_fixed;
    Vec2d m_anchor;
  };

  class EnvElem
  {
  public:
    EnvElem(EnvironmentMap* ptr):
      gEnv(ptr)
    {

    }

    Vec2i getMapCoord() { return m_pos; }
    Vec2d getGlobalCoord() { return gEnv->mapToGlobal(m_pos); }

  protected:
    EnvironmentMap *gEnv;
    Vec2i m_pos;
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

    json toJSON();

  protected:
    int m_id, m_lm_n;

    map_octree_t m_root;

    Vec2d m_gps_rect[4];
  };


  /** Manages collections of local navigation meshes lazily loaded when necessary **/
  class NavmeshComposer
  {
  public:
    NavmeshComposer(Navigator*);

    //TODO: Dynamic area support
    int setStaticArea(rect_t);

    //int addBounds(rect_t, EnvironmentMap*);
    int addVirtualBounds(const std::vector<Vec2i>*);

    void clearBounds();
    void clearVirtBounds();

    image_t* getLocalFrame() { return &m_frame; }

    void updateLocalFrame(Vec2i pos);

  protected:
    Navigator *gNav;
    rect_t m_rect;  // Area relative to current pose position

    image_t m_bL, m_vbL;
    image_t m_frame;
  };

  class Pather
  {
  public:
    typedef Vec2i wp_t;
    typedef std::vector<wp_t> path_t;

  public:
    Pather();

    int init(rect_t);

    void poseTick(Vec2i, float);
    wp_t pathTick(Vec2i, float);

  protected:
    bool m_valid_path;
    path_t m_path;

    rect_t m_bounds;

    wp_t m_pos;
    float m_ori;

    wp_t m_tPos;
    float m_tOri;
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

    NavmeshComposer* getComposer() { return &mComposer; }

    Pather* getPather() { return &mPather; }

    int init();

    void setup();
    void process();

    int navTo(const Vec2i&);
    int navTo(const Vec2i&, const float&);
    // int navTo(const float&);

    int isReachable(Vec2i);

    int setBounds(std::vector<Vec2i>*);

    ctx_t getContext() { return m_ctx; }

    int feedLandmarks(LandmarkSet*);

    int getLocalMaps(EnvMapPtrSet&);

    int globalToMap(const Vec2d&, Vec2i);
    int mapToGlobal(const Vec2i&, Vec2d);

  protected:
    bool m_init;
    ctx_t m_ctx;

    Localizer *gLoc;

    NavmeshComposer mComposer;
    Vec2d m_local_anchor;

    OctreeLandmarkMap mMap;
    Pather mPather;
  };

}

#endif
