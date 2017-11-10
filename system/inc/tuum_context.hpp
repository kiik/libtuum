
#ifndef TUUM_CONTEXT_H
#define TUUM_CONTEXT_H

namespace tuum {

  class Visioning;
  class System;
  class Motion;
  class Navigator;

  // Deprecated
  class Navigation;

  extern Visioning* gVision;
  extern Navigator* gNav;

  extern System* gSystem;

  void setGlobalSystem(System*);

}

#endif
