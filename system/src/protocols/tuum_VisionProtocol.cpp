
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "base64.hpp"

#include "hal.hpp"

#include "lpx_iformat.hpp"
#include "tuum_visioning.hpp"
#include "tuum_system.hpp"

#include "protocols/tuum_VisionProtocol.hpp"

using namespace tuum::wsocs;

namespace tuum {

  VisionProtocol::VisionProtocol():
    WSProtocol({
      "Vision Protocol",
      "/vision",
      "0.0.1-al.0",
      {
        {"Read Frame", "/frame", {
            {"StreamName", "sn", WSType::WST_String},
          }
        },
      },
      this
    })
  {

  }

  const char* JS_METHOD = "m";
  const char* JS_MTH_GET = "get";
  const char* JS_MTH_PUT = "put";
  const char* JS_DATA = "data";

  int VisionProtocol::route(const WSProtocol::Message& m) {
    std::string uri = m.dat[WSProtocol::JS_URI].get<std::string>();

    if(uri == "getFrame") {
      return getFrame(m.dat);
    } else if (uri == "settings") {
      return toggleThresholding(m.dat);
    } else if (uri == "filters") {
      if(m.dat[JS_METHOD].get<std::string>() == JS_MTH_GET) {

      } else if(m.dat[JS_METHOD].get<std::string>() == JS_MTH_PUT) {

      }
    } else if(uri == "pplcnf") {
      return vConfig(m.dat);
    } else if(boost::starts_with(uri, "/vfilter")) {
      return vFilter(m.dat);
    } else if(uri == "ent_get") {
      return getEntities();
    }

    return -1;
  }

  //TODO: Error catching
  int VisionProtocol::getFrame(const json& dat) {
    if(dat["dev"].get<std::string>() == "CAM0") {
      //hal::hw.
      json res;

      /*
      tuum::Visioning* vis = tuum::gVision;
      if(vis == nullptr) return -1;

      image_t img;
      Env::spawnBuffer(img);

      if(vis->readFrame(img) < 0) return -2;

      image_t out = lpx::rgb_to_png(img);

      std::string b64_img = b64::encode((const char*)out->data, out->size);
      */
      res["frame"] = "";

      send(res);
      return 0;
    }

    return -1;
  }

  int VisionProtocol::toggleThresholding(const json& dat) {
    /*
    if(dat["threshold"].get<bool>() == true)
      tuum::gVision->setThresholding(true);
    else
      tuum::gVision->setThresholding(false);
    */
    return 0;
  }

  int VisionProtocol::vConfig(const json& dat) {
    /*
    if(dat.find("data") != dat.end())
      tuum::gVision->configure(dat["data"]);
      */
    json out;
    //tuum::gVision->toJSON(out);
    send(out);

    return 0;
  }

  int VisionProtocol::vFilter(const json& dat) {
    std::string uri = dat[WSProtocol::JS_URI].get<std::string>();

    if(uri == "/vfilter_get") {
      json res;
      //tuum::gVision->getFilter()->toJSON(res);
      res["_r"] = "OK";
      send(res);
    } else if(uri == "/vfilter_set") {
      auto r = dat["f"]["range"];
      VisionFilter::ColorClass cls = {r[0], r[1], r[2], r[3], r[4], r[5]};
      cls.id = dat["f"]["id"];
      printf("SET %s\n", dat.dump().c_str());
      //tuum::gVision->getFilter()->updateYUVClassifier(cls);

      json res;
      res["_r"] = "OK";
      send(res);
    }

    return 0;
  }

  int VisionProtocol::getEntities() {
    json dat;
    //tuum::gSystem->getEntityHandle()->toJSON(dat);
    send(dat);
    return 0;
  }

}
