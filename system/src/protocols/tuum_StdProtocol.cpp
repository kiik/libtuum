
#include "tuum_context.hpp"

#include "tuum_localizer.hpp"
#include "tuum_navigator.hpp"

#include "protocols/tuum_StdProtocol.hpp"

using namespace tuum::wsocs;

namespace tuum {

  TuumStdProtocol::TuumStdProtocol():
    WSProtocol({
      "Tuum Std Protocol",
      "/std",
      "0.0.1-al.0",
      {
        {"Get Global Position", "/gps", {}},
        {"Get Local Pose", "/pose", {}},
        {"Get Local Maps", "/maps", {}},
      },
      this
    })
  {

  }

  int TuumStdProtocol::getGlobalPosition(json& out)
  {
    if(gSystem == nullptr) return -1;

    Localizer *ptr = (Localizer*)gSystem->findSubsystem(Localizer::GetType());
    if(ptr == nullptr) return -2;

    Vec2d pos;
    int res = ptr->getGlobalPosition(pos);

    out["lat"] = pos.x;
    out["lng"] = pos.y;
    out["res"] = res;

    return 0;
  }

  int TuumStdProtocol::getLocalPose(json& out)
  {
    if(gSystem == nullptr) return -1;

    Localizer *ptr = (Localizer*)gSystem->findSubsystem(Localizer::GetType());
    if(ptr == nullptr) return -2;

    localized_pose_t pose;
    int res = ptr->getLocalPose(pose);

    out["coord"] = {pose.coord.x, pose.coord.y};
    out["orient"] = pose.orient;
    out["mapId"] = pose.map_id;
    out["res"] = res;

    return 0;
  }

  int TuumStdProtocol::getLocalMaps(json& out)
  {
    if(gSystem == nullptr) return -1;

    Navigator *ptr = (Navigator*)gSystem->findSubsystem(Navigator::GetType());
    if(ptr == nullptr) return -2;

    EnvMapPtrSet maps;
    int res = ptr->getLocalMaps(maps);

    out["maps"] = json::array();

    for(auto it = maps.begin(); it != maps.end(); ++it) {
      out["maps"].push_back((*it)->toJSON());
    }

    return 0;
  }

  int TuumStdProtocol::route(const WSProtocol::Message& m) {
    std::string uri = m.dat[WSProtocol::JS_URI].get<std::string>();

    if(uri == "/gps") return reqGlobalPosition();
    if(uri == "/pose") return reqLocalPose();
    if(uri == "/maps") return reqLocalMaps();

    return -1;
  }

  int TuumStdProtocol::reqGlobalPosition()
  {
    json out;

    int res = getGlobalPosition(out);
    if(res < 0) {
      out["res"] = res;
    }

    return send(out);
  }

  int TuumStdProtocol::reqLocalPose()
  {
    json out;

    int res = getLocalPose(out);
    if(res < 0) {
      out["res"] = res;
    }

    return send(out);
  }

  int TuumStdProtocol::reqLocalMaps()
  {
    json out;

    int res = getLocalMaps(out);
    if(res < 0) {
      out["res"] = res;
    }

    return send(out);
  }

}
