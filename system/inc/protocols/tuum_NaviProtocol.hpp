
#ifndef TUUM_NAVPROTO_H
#define TUUM_NAVPROTO_H

#include "tuum_wsproto.hpp"

using namespace tuum::wsocs;

namespace tuum {

  class NaviProtocol : public WSProtocol
  {
  public:
    NaviProtocol();

    int route(const WSProtocol::Message&);

    int reqPerception(const json&);
    int reqMap(const json&);
    int reqNavmesh(const json&);

    int reqEnv(const json&);
    int reqNavmap(const json&);

  private:

  };

}

#endif
