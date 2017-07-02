/** @name tuum_system.hpp
 *  @brief 'System' class encapsulating robot subsystems and handling their management.
 *
 *  @author Meelik Kiik
 */

#ifndef TUUM_SYSTEM_H
#define TUUM_SYSTEM_H

#include "json.hpp"

#include "tuum_EntityFilter.hpp"

using json = nlohmann::json;

#define TUUM_SUBSYS_DECL(cls) \
  static Subsystem::TypeVar Type; \
  static Subsystem::Type GetType() { return &cls::Type; }

#define TUUM_SUBSYS_IMPL(cls) \
  Subsystem::TypeVar cls::Type;

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

    template<typename T>
    int getSubsystemHandle(Type t, T& out) {
      out = (T)findSubsystem(t);

      if(out == nullptr) return -1;
      return 1;
    }

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

}

namespace tuum {

  class System
  {
  public:
    System();

    void setup();
    void process();

    size_t insmod(Subsystem*);

    Subsystem* findSubsystem(Subsystem::Type);

    bool entityPassEnabled();

  protected:
    SubsysPtrSet mSubsystems;
  };

}

#endif
