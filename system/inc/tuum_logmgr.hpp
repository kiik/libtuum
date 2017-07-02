
#ifndef TUUM_LOGMAN_H
#define TUUM_LOGMAN_H

#include "Vec.hpp"

#include "tuum_system.hpp"

namespace tuum {

  struct logic_impl_t {
    int id;
    std::string clsName, objName;
  };


  class LogicImpl
  {
  public:
    static int id_seq;

  public:
    LogicImpl(const std::string&, const std::string&);

    virtual int init() = 0;

    virtual void setup() = 0;
    virtual void process() = 0;

    virtual void pause() = 0;
    virtual void terminate() = 0;

    virtual bool isRunnable() { return false; }
    virtual bool isInterrupted() { return false; }
    virtual bool isComplete() { return false; }

    virtual int configure(const json&) { return 0; }

    virtual json serialize() {};
    virtual int deserialize(json&) {};

    virtual json getStatusJSON() {
      json out = json::object();
      return out;
    };

    logic_impl_t getDescriptor() { return m_desc; }

    json getDescriptorJSON();

  protected:
    logic_impl_t m_desc;
  };


  class LogicMgr : public Subsystem
  {
  public:
    TUUM_SUBSYS_DECL(LogicMgr);

    enum LogicMgrStateE {
      LMS_IDLE,   // No job
      LMS_INIT,   // New job
      LMS_READY,  // Configured job
      LMS_RUN,    // Running job
      LMS_PAUSE,  // Paused job
      LMS_INT,    // Interrupted job
      LMS_TERM,   // Terminated job
      LMS_COMPL,  // Completed job
    };

    static std::string StateString(LogicMgrStateE);

  public:
    LogicMgr();

    int init();

    void setup();
    void process();

    Subsystem::Type getType() { return LogicMgr::GetType(); }

    bool getState() { return m_state; }

    int setLogicImpl(LogicImpl*);

    LogicImpl* getLogicImpl() { return mActiveLogic; }

    int prepareRun();
    int beginRun();
    int pauseRun();
    int stopRun();

    void onInterrupt();
    void onTerminate();
    void onComplete();

    json toJSON();

  protected:
    void transition(LogicMgrStateE);

    LogicMgrStateE m_state;
    LogicImpl* mActiveLogic;
  };

}

#endif
