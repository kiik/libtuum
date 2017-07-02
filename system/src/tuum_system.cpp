
#include <boost/thread.hpp>

#include "tuum_hal.hpp"
#include "tuum_lpx.hpp"

#include "tuum_system.hpp"

namespace tuum {

  size_t Subsystem::id_seq = 1;

  Subsystem::Subsystem():
    mParent(nullptr)
  {
    mId = Subsystem::id_seq++;
    mName = format("<Subsystem id=%lu>", mId);
  }

  Subsystem::Subsystem(const char* name):
    mParent(nullptr)
  {
    mId = Subsystem::id_seq++;
    mName = format("<Subsystem/%s id=%lu>", name, mId);
  }

  void Subsystem::setParent(System* ptr) { mParent = ptr; }

  Subsystem* Subsystem::findSubsystem(Subsystem::Type t) {
    return mParent->findSubsystem(t);
  }

  int Subsystem::setProperty(const std::string& pname, const json& pval)
  {
    return -1;
  }

  std::string Subsystem::getName()
  {
    return mName;
  }

  json Subsystem::getStats() {
    json out = json::object();
    return out;
  }

  json Subsystem::getProperties() {
    json out = json::object();
    return out;
  }


  System::System()
  {

  }

  void System::setup()
  {
    hal::setup();

    tuum::lpx::init();
    tuum::lpx::setup();

    for(auto it = mSubsystems.begin(); it != mSubsystems.end(); it++)
      (*it)->setup();

    //mVision.init();
    //mMotion.init();

    //if(gMotionThr == nullptr)
      //gMotionThr = new boost::thread(boost::bind(&Motion::loop, &mMotion));

    RTXLOG("Setup done.");
  }

  size_t System::insmod(Subsystem* ptr) {
    ptr->setParent(this);
    mSubsystems.push_back(ptr);

    RTXLOG(format("registered '%s'", ptr->getName()));

    return mSubsystems.size() - 1;
  }

  Subsystem* System::findSubsystem(Subsystem::Type t) {
    for(auto it = mSubsystems.begin(); it != mSubsystems.end(); it++) {
      if((*it)->getType() == t) return *it;
    }
    return nullptr;
  }

  void System::process()
  {
    hal::process();

    for(auto it = mSubsystems.begin(); it != mSubsystems.end(); it++)
      (*it)->process();

    return;

    /*
#ifdef TUUM_SYS_BENCH
    size_t t0, t1, t2, t3, t4, t5;
#endif

#ifdef TUUM_SYS_BENCH
    t0 = millis();
#endif

    hal::process();
    lpx::process();

#ifdef TUUM_SYS_BENCH
    t1 = millis();
#endif

    mMotion.run();

#ifdef TUUM_SYS_BENCH
    t2 = millis();
#endif

    mVision.run();

#ifdef TUUM_SYS_BENCH
    t3 = millis();
#endif

    if(entityPassEnabled())
      mEntityFilter.digest(mVision.getFilter()->getBlobs());

#ifdef TUUM_SYS_BENCH
    t4 = millis();
#endif

    mEntityFilter.process();

#ifdef TUUM_SYS_BENCH
    t5 = millis();
#endif

#ifdef TUUM_SYS_BENCH
    printf("[System - stat]ll %lums, mot %lums, vis %lums, ef-digest %lums, ef %lums\n", (t1 - t0), (t2 - t1), (t3 - t2), (t4 - t3), (t5 - t4));
#endif
    */
  }


  bool System::entityPassEnabled() {
    // if(!mVision.thresholdPassEnabled()) return false;
    return false;
  }

}
