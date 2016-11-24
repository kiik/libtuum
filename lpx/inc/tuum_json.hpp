
#ifndef TUUM_JSON_H
#define TUUM_JSON_H

#include "json.hpp"

using json = nlohmann::json;

namespace tuum {

  template<class T>
  class JSONSerializable {
  public:

    virtual void toJSON(std::string& out) {
      json dat;
      toJSON(dat);
      out = dat.dump();
    };

    virtual void fromJSON(std::string in) {
      json dat = json::parse(in);
      this->fromJSON(dat);
    };

    virtual void toJSON(json&) {};
    virtual void fromJSON(json) {};

  };

}

#endif
