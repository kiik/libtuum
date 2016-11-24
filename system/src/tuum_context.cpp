
#include "tuum_context.hpp"

#include "tuum_visioning.hpp"
#include "tuum_system.hpp"

namespace tuum {

  Visioning* gVision = nullptr;
  System* gSystem = nullptr;

  void setGlobalSystem(System* ptr)
  {
    gSystem = ptr;
    gVision = ptr->getVisioning();
  }

}
