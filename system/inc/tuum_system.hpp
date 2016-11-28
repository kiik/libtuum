
#ifndef TUUM_SYSTEM_H
#define TUUM_SYSTEM_H

#include "tuum_EntityFilter.hpp"

#include "tuum_motion.hpp"
#include "tuum_physics.hpp"
#include "tuum_visioning.hpp"
#include "tuum_navigation.hpp"

namespace tuum {

  class System
  {
  public:
    System();

    void setup();
    void process();

    Visioning* getVisioning() { return &mVision; }
    Motion* getMotioning() { return &mMotion; }

    EntityFilter* getEntityHandle() { return &mEntityFilter; }

    Physics* getPhysics() { return &mPhysics; }
    Navigation* getNavi() { return &mNavi; }

    bool entityPassEnabled();

  protected:
    Visioning mVision;
    EntityFilter mEntityFilter;

    Physics mPhysics;
    Navigation mNavi;

    Motion mMotion;
  };

}

#endif
