
#include "tuum_logmgr.hpp"

namespace tuum {

  int LogicImpl::id_seq = 0;

  TUUM_SUBSYS_IMPL(LogicMgr);

  LogicImpl::LogicImpl(const std::string& clsn, const std::string& objn):
    m_desc({++LogicImpl::id_seq, clsn, objn})
  {
    RTXLOG(tuum::format("New job <%s id=%i, '%s'>", m_desc.clsName, m_desc.id, m_desc.objName));
  }

  json LogicImpl::getDescriptorJSON() {
    json out = json::object();

    out["id"] = m_desc.id;
    out["cls"] = m_desc.clsName;
    out["name"] = m_desc.objName;

    return out;
  }

  std::string LogicMgr::StateString(LogicMgrStateE state)
  {
    switch(state) {
      case LMS_IDLE: return "IDLE";
      case LMS_INIT: return "INIT";
      case LMS_READY: return "READY";
      case LMS_RUN: return "RUNNING";
      case LMS_PAUSE: return "PAUSED";
      case LMS_INT: return "INTERRUPT";
      case LMS_TERM: return "TERMINATED";
      case LMS_COMPL: return "COMPLETED";
    }
  }

  LogicMgr::LogicMgr():
    m_state(LMS_IDLE),
    mActiveLogic(nullptr)
  {

  }

  void LogicMgr::transition(LogicMgrStateE next_state)
  {
    RTXLOG(tuum::format("transition '%s' -> '%s'", LogicMgr::StateString(m_state), LogicMgr::StateString(next_state)));
    m_state = next_state;
  }

  int LogicMgr::init()
  {

  }

  void LogicMgr::setup()
  {

  }

  void LogicMgr::process()
  {
    switch(m_state) {
      case LMS_INIT:
        break;
      case LMS_RUN:
        if(mActiveLogic != nullptr) {
          if(!mActiveLogic->isRunnable()) onTerminate();
          mActiveLogic->process();

          if(mActiveLogic->isComplete()) onComplete();
          else if(mActiveLogic->isInterrupted()) onInterrupt();
        }
        break;
      case LMS_INT:
        //TODO: Periodic operator notification
        break;
      case LMS_TERM:
        //TODO: Periodic operator notification
        break;
      case LMS_COMPL:
        //TODO: Periodic operator notification
        break;
    }
  }

  int LogicMgr::setLogicImpl(LogicImpl* ptr)
  {
    if(mActiveLogic != nullptr) {
      RTXLOG("mActiveLogic != nullptr", LOG_ERR);
      return -1;
    }

    if(m_state != LMS_IDLE) {
      RTXLOG("m_state != LMS_IDLE", LOG_ERR);
      return -2;
    }

    mActiveLogic = ptr;
    transition(LMS_INIT);

    return 1;
  }

  int LogicMgr::prepareRun()
  {
    if(mActiveLogic == nullptr) return -1;

    if(m_state == LMS_PAUSE) {
      if(!mActiveLogic->isRunnable()) {
        transition(LMS_INT);
        return -3;
      }
    }

    if((m_state != LMS_INIT)) return -2;
    transition(LMS_READY);

    return 1;
  }

  int LogicMgr::beginRun()
  {
    if(mActiveLogic == nullptr) return -1;
    if(m_state != LMS_READY) return -2;

    mActiveLogic->setup();
    transition(LMS_RUN);

    return 1;
  }

  int LogicMgr::pauseRun()
  {
    if(m_state != LMS_RUN) return -1;

    mActiveLogic->pause();
    transition(LMS_PAUSE);

    return 1;
  }

  int LogicMgr::stopRun()
  {
    if((m_state != LMS_RUN) && (m_state != LMS_PAUSE))
      return -1;

    mActiveLogic->terminate();
    transition(LMS_TERM);

    return 1;
  }

  void LogicMgr::onInterrupt() {
    transition(LMS_INT);
    //TODO: Notify operator
  }

  void LogicMgr::onTerminate() {
    transition(LMS_TERM);
    //TODO: Notify operator
  }

  void LogicMgr::onComplete() {
    transition(LMS_COMPL);
    //TODO: Notify operator
  }

  json LogicMgr::toJSON() {
    json out;

    out["st"] = LogicMgr::StateString(m_state);

    if(mActiveLogic != nullptr) {
      out["job"] = mActiveLogic->getDescriptorJSON();
      out["ctx"] = mActiveLogic->getStatusJSON();
    } else {
      out["job"] = json::object();
    }

    return out;
  }

}
