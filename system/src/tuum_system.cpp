
#include <boost/thread.hpp>

#include "tuum_hal.hpp"
#include "tuum_lpx.hpp"

#include "tuum_system.hpp"

namespace tuum {

  boost::thread* gMotionThr = nullptr;

  System::System():
    mPhysics(&mEntityFilter),
    mNavi(&mEntityFilter)
  {

  }

  void System::setup()
  {
    hal::setup();

    tuum::lpx::init();
    tuum::lpx::setup();

    mVision.init();
    mMotion.init();

    //if(gMotionThr == nullptr)
      //gMotionThr = new boost::thread(boost::bind(&Motion::loop, &mMotion));
  }


  void System::process()
  {
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
  }


  bool System::entityPassEnabled() {
    if(!mVision.thresholdPassEnabled()) return false;
    return true;
  }

}
