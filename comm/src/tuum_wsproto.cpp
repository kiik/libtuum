
#include "wsocs/WebSocketServer.hpp"

#include "tuum_wsproto.hpp"

namespace tuum { namespace wsocs {

  const char* WSProtocol::JS_URI = "uri";
  const char* WSProtocol::JS_CMD = "c";
  const char* WSProtocol::JS_M_ID = "_";

  WSProtocol::WSProtocol():
    mWS(nullptr),
    route_id_seq(1)
  {

  }

  void WSProtocol::setWS(WebSocketServer* ws) {
    mWS = ws;
  }

  int WSProtocol::send(json& dat) {
    if(mWS == nullptr) return -1;
    mWS->send(dat);
    return 0;
  }

  int WSProtocol::recv(Packet _dat) {

  }

  int WSProtocol::validate(Packet* p) {
    //TODO
  }

  /*
  int WSProtocol::route(Packet* p) {

  }*/

  int WSProtocol::route(const Message& m) {
    std::string uri = m.dat["uri"];

    auto it = mRouteMap.find(uri);
    if(it == mRouteMap.end()) return -1;

    return it->second.wsp->route(m);
  }

  size_t WSProtocol::add(route_t ro)
  {
    if(ro.uri == "") return 0;

    ro.id = route_id_seq++;
    mRouteMap[ro.uri] = ro;

    RTXLOG(format("Registered '%s', id=%lu.", ro.uri, ro.id));
    return ro.id;
  }

  void WSProtocol::remove(size_t id)
  {
    //TODO:
  }

  WSProtocol::route_t WSProtocol::getDescriptor() {
    route_t ro;
    return ro;
  }


}}
