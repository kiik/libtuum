
#include "tuum_context.hpp"

#include "tuum_visioning.hpp"
#include "tuum_system.hpp"

namespace tuum {

  Visioning* gVision = nullptr;
  Navigation* gNavi = nullptr;
  Motion* gMotion = nullptr;

  System* gSystem = nullptr;

  void setGlobalSystem(System* ptr)
  {
    gSystem = ptr;

    gVision = ptr->getVisioning();
    gNavi   = ptr->getNavi();
    gMotion = ptr->getMotioning();
  }

}
