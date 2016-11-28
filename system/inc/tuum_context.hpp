
#ifndef TUUM_CONTEXT_H
#define TUUM_CONTEXT_H

namespace tuum {

  class Visioning;
  class System;
  class Motion;
  class Navigation;

  extern Visioning* gVision;
  extern Motion* gMotion;

  extern Navigation* gNavi;

  extern System* gSystem;

  void setGlobalSystem(System*);

}

#endif
