
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

        {"Get Pose", "/pose", {}},
        {"Get Local Pose", "/lpo", {}},
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

    tuum::gps_t pos;
    int res = ptr->getGlobalPosition(pos);

    out["lat"] = pos.lat;
    out["lng"] = pos.lon;
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

  int TuumStdProtocol::getPose(json& out)
  {
    if(gSystem == nullptr) return -1;

    Localizer *loc = (Localizer*)gSystem->findSubsystem(Localizer::GetType());
    if(loc == nullptr) return -2;

    Navigator *nav = (Navigator*)gSystem->findSubsystem(Navigator::GetType());
    if(nav == nullptr) return -2;

    localized_pose_t pose;

    int res = loc->getLocalPose(pose);

    auto ctx = nav->getContext();

    json buf = json::object();

    buf["pos"] = {pose.coord.x, pose.coord.y};
    buf["ori"] = pose.orient;

    if(ctx.hasTarget()) buf["tPos"] = {{"x", ctx.tPos.x}, {"y", ctx.tPos.y}};
    else nullptr;

    if(ctx.hasOrient()) buf["tOri"] = ctx.tOri;
    else nullptr;

    buf["mapId"] = pose.map_id;
    buf["_t"] = pose._t;

    out["pose"] = buf;

    gps_t gpos;
    loc->getGlobalPosition(gpos);
    out["gpos"] = {{"lat", gpos.lat}, {"lng", gpos.lon}};


    out["res"] = res;

    return 0;
  }

  int TuumStdProtocol::route(const WSProtocol::Message& m) {
    std::string uri = m.dat[WSProtocol::JS_URI].get<std::string>();

    if(uri == "/gps") return reqGlobalPosition();
    if(uri == "/pose") return reqPose();
    if(uri == "/lpo") return reqLocalPose();
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

  int TuumStdProtocol::reqPose()
  {
    json out;
    getPose(out);
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
