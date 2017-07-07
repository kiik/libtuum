
#ifndef TUUM_STD_WS_PROTOCOL_H
#define TUUM_STD_WS_PROTOCOL_H

#include "tuum_wsproto.hpp"

using namespace tuum::wsocs;

namespace tuum {

  class TuumStdProtocol : public WSProtocol
  {
  public:
    TuumStdProtocol();

    int getGlobalPosition(json&);
    int getLocalPose(json&);

    int getLocalMaps(json&);
    int getMapData(json&);

    int getPose(json&);

    int route(const WSProtocol::Message&);

    int reqGlobalPosition();
    int reqLocalPose();
    int reqPose();

    int reqLocalMaps();
    int reqMapData();
  };

}

#endif
