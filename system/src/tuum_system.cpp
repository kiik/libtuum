
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
    hal::process();
    lpx::process();

    mMotion.run();

    mVision.run();

    if(entityPassEnabled())
      mEntityFilter.digest(mVision.getFilter()->getBlobs());

    mEntityFilter.process();
  }


  bool System::entityPassEnabled() {
    if(!mVision.thresholdPassEnabled()) return false;
    return true;
  }

}
