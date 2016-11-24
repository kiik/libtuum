
#ifndef TUUM_CONTEXT_H
#define TUUM_CONTEXT_H

namespace tuum {

  class Visioning;
  class System;

  extern Visioning* gVision;
  extern System* gSystem;

  void setGlobalSystem(System*);

}

#endif
