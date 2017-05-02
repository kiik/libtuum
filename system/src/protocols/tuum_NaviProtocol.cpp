
#include "hal.hpp"

#include "tuum_system.hpp"

#include "protocols/tuum_NaviProtocol.hpp"

using namespace tuum::wsocs;

namespace tuum {

  NaviProtocol::NaviProtocol():
    WSProtocol({
      "Navigation Protocol",
      "/nav",
      "0.0.1-al.0",
      {
        {"Get Perception", "/perc", {}},
        {"Get Map", "/map", {}},
        {"Get Navmesh", "/navmesh", {}},

        {"Get Environment", "/env", {}},
        {"Get Navmap", "/navmap", {}},
      },
      this
    })
  {

  }

  const char* JS_METHOD = "m";
  const char* JS_MTH_GET = "get";
  const char* JS_MTH_PUT = "put";
  const char* JS_DATA = "data";

  int NaviProtocol::route(const WSProtocol::Message& m) {
    std::string uri = m.dat[WSProtocol::JS_URI].get<std::string>();

    if(uri == "/perc") {
      return reqPerception(m.dat);
    } else if (uri == "/map") {
      return reqMap(m.dat);
    } else if (uri == "/navmesh") {
      return reqNavmesh(m.dat);
    } else if(uri == "/env") {
      return reqNavmap(m.dat);
    }

    return -1;
  }

  int NaviProtocol::reqPerception(const json& dat)
  {
    json out = json::object();
    out["msg"] = "TODO";
    return send(out);
  }

  int NaviProtocol::reqMap(const json& dat)
  {
    json out = json::object();
    out["msg"] = "TODO";
    return send(out);
  }

  int NaviProtocol::reqNavmesh(const json& dat)
  {
    json out = json::object();
    out["msg"] = "TODO";
    return send(out);
  }

  int NaviProtocol::reqEnv(const json& dat)
  {
    json out = json::object();
    out["msg"] = "TODO";
    return send(out);
  }

  int NaviProtocol::reqNavmap(const json& dat)
  {
    json out = json::object();
    out["msg"] = "TODO";
    return send(out);
  }

}
