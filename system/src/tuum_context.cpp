
#include "tuum_context.hpp"

#include "tuum_visioning.hpp"
#include "tuum_system.hpp"

namespace tuum {

  System* gSystem = nullptr;

  void setGlobalSystem(System* ptr)
  {
    gSystem = ptr;
  }

}
