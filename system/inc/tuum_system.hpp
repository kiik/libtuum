
#ifndef TUUM_SYSTEM_H
#define TUUM_SYSTEM_H

#include "tuum_EntityFilter.hpp"

#include "tuum_physics.hpp"
#include "tuum_visioning.hpp"

namespace tuum {

  class System
  {
  public:
    System();

    void setup();
    void process();

    Visioning* getVisioning() { return &mVision; }
    Physics* getPhysics() { return &mPhysics; }

    bool entityPassEnabled();

  protected:
    Visioning mVision;
    EntityFilter mEntityFilter;
    Physics mPhysics;
  };

}

#endif
