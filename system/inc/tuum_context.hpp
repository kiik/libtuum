
#ifndef TUUM_CONTEXT_H
#define TUUM_CONTEXT_H

namespace tuum {

  class Visioning;
  class System;
  class Motion;

  extern Visioning* gVision;
  extern Motion* gMotion;

  extern System* gSystem;

  void setGlobalSystem(System*);

}

#endif
