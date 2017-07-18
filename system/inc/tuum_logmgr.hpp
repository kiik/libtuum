
#ifndef TUUM_LOGMAN_H
#define TUUM_LOGMAN_H

#include "Vec.hpp"

#include "tuum_system.hpp"

namespace tuum {

  class Agent;

  struct logic_impl_t {
    int id;
    std::string clsName, objName;
  };

  class LogicMgr;

  class LogicImpl
  {
  public:
    friend class LogicMgr;

    static int id_seq;

  public:
    LogicImpl(const std::string&, const std::string&, LogicMgr*);

    virtual int init() = 0;

    virtual void setup() = 0;
    virtual void process() = 0;

    // State signals
    virtual int pause() = 0;
    virtual int resume() = 0;
    virtual int terminate() = 0;

    // Flag getters
    virtual bool isRunnable() { return false; }
    virtual bool isInterrupted() { return false; }
    virtual bool isComplete() { return false; }

    // Init methods
    virtual int configure(const json&, json&) { return 0; }
    virtual int finalize() { return -1; }

    virtual json serialize() {};
    virtual int deserialize(json&) {};

    logic_impl_t getDescriptor() { return m_desc; }

    json getDescriptorJSON();

    // JSON API
    virtual json getOverviewJSON() = 0; // State data
    virtual json getConfJSON() = 0;     // Input data
    virtual json getContextJSON() = 0;  // Runtime data

  protected:
    LogicMgr* m_parent;
    logic_impl_t m_desc;
  };

  enum LogicMgrStateE {
    LMS_IDLE,   // No job
    LMS_INIT,   // New job
    LMS_READY,  // Configured job
    LMS_BEGIN,
    LMS_RUN,    // Running job
    LMS_PAUSE,  // Paused job
    LMS_INT,    // Interrupted job
    LMS_TERM,   // Terminated job
    LMS_COMPL,  // Completed job
  };

  class LogicMgr : public Subsystem
  {
  public:
    TUUM_SUBSYS_DECL(LogicMgr);

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
    int resumeRun();
    int stopRun();

    void onInterrupt();
    void onTerminate();
    void onComplete();

    bool canEnter(LogicMgrStateE);

    json toJSON();

  protected:
    void transition(LogicMgrStateE);
    
    bool m_init;

    LogicMgrStateE m_state;
    LogicImpl* mActiveLogic;

    Agent *gAgent;
  };

}

#endif
