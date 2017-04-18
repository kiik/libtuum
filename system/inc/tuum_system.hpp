/** @name tuum_system.hpp
 *  @brief 'System' class encapsulating robot subsystems and handling their management.
 *
 *  @author Meelik Kiik
 */

#ifndef TUUM_SYSTEM_H
#define TUUM_SYSTEM_H

#include "json.hpp"

#include "tuum_EntityFilter.hpp"

#include "tuum_motion.hpp"
#include "tuum_physics.hpp"
#include "tuum_visioning.hpp"
#include "tuum_navigation.hpp"

using json = nlohmann::json;

namespace tuum {

  class System;

  class Subsystem
  {
  protected:
    static size_t id_seq;

  public:
    typedef int TypeVar;
    typedef TypeVar* Type;

    virtual Type getType() { return nullptr; }

  public:
    Subsystem();
    Subsystem(const char*);

    void setParent(System*);

    Subsystem* findSubsystem(Type t);

    int setProperty(const std::string&, const json&); // pname, val
    json getProperty();

    virtual std::string getName();

    virtual json getStats();
    virtual json getProperties();

    virtual void setup() {}
    virtual void process() {}

  protected:
    System* mParent;

    size_t mId;
    std::string mName;
    json mProps;
  };

  typedef std::vector<Subsystem*> SubsysPtrSet;

  class System
  {
  public:
    System();

    void setup();
    void process();

    size_t insmod(Subsystem*);

    Subsystem* findSubsystem(Subsystem::Type);

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

    SubsysPtrSet mSubsystems;
  };

}

#endif
