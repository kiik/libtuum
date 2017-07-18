
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
        {"Get Nav Path", "/navpath", {}}
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

    if(uri == "/navpath") return reqPath(m.dat);

    return -1;
  }

  int NaviProtocol::getPath(json& out)
  {
    out["path"] = json::array();

    for(int i=0; i < 2; i++) {
      json buf = json::object();

      buf["mId"] = 0;
      buf["wps"] = json::array({{10, 10}, {20, 20}, {30, 30}});

      out["path"].push_back(buf);
    }

    return 0;
  }

  int NaviProtocol::reqPath(const json& dat)
  {
    json out = json::object();
    int res = getPath(out);

    out["res"] = res;

    return send(out);
  }

}
