
#include "wsocs/WebSocketServer.hpp"

#include "tuum_wsproto.hpp"

namespace tuum { namespace wsocs {

  const char* WSProtocol::JS_M_ID = "_";
  const char* WSProtocol::JS_NSP = "nsp";
  const char* WSProtocol::JS_URI = "uri";

  WSProtocol::WSProtocol():
    mWS(nullptr),
    route_id_seq(1),
    m_wsp({"", "", "", {}, nullptr})
  {

  }

  WSProtocol::WSProtocol(const wsp_t wsp):
    mWS(nullptr),
    route_id_seq(1),
    m_wsp(wsp)
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

  json WSProtocol::getProtocols() {
    json out = json::array();

    for(auto it = mRouteMap.begin(); it != mRouteMap.end(); it++) {
      out.push_back(it->second.wsp->getDescriptor()->toJSON());
    }

    return out;
  }

  int WSProtocol::route(const Message& m) {
    if(m.dat.find("nsp") == m.dat.end()) return -1;

    std::string nsp = m.dat["nsp"];

    if(nsp == "*") {
      json dat = json::object();
      dat["protocols"] = getProtocols();
      send(dat);
      return 1;
    }

    auto it = mRouteMap.find(nsp);
    if(it == mRouteMap.end()) return -1;

    return it->second.wsp->route(m);
  }

  size_t WSProtocol::add(const wsp_t* wsp)
  {
    route_t ro;
    ro.id = route_id_seq++;
    ro.uri = wsp->uri;
    ro.wsp = wsp->handler;

    if(mRouteMap.find(ro.uri) != mRouteMap.end()) {
      RTXLOG(format("Overwriting uri '%s'...", ro.uri), LOG_WARN);
    }

    mRouteMap[ro.uri] = ro;

    RTXLOG(format("Registered '%s', id=%lu.", ro.uri, ro.id));
    return ro.id;
  }

  void WSProtocol::remove(size_t id)
  {
    //TODO:
  }

  wsp_t* WSProtocol::getDescriptor() {
    return &m_wsp;
  }

}}
