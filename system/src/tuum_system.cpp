
#include "tuum_hal.hpp"
#include "tuum_lpx.hpp"

#include "tuum_system.hpp"

namespace tuum {

  System::System():
    mPhysics(&mEntityFilter)
  {

  }

  void System::setup()
  {
    hal::setup();

    tuum::lpx::init();
    tuum::lpx::setup();

    mVision.init();
  }

  void System::process()
  {
    hal::process();
    lpx::process();
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
