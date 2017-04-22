
#ifndef TUUM_WSPROTO_H
#define TUUM_WSPROTO_H

#include <string>
#include <sstream>
#include <map>

#include <libwebsockets.h>

#include "json.hpp"

#include "platform.hpp"

#include "wsocs/declarations.hpp"

using json = nlohmann::json;

namespace tuum {
namespace wsocs {

  class WSProtocol;

  enum WSType {
    WST_Integer,
    WST_String,
  };

  struct wsp_arg_t {
    std::string name;
    std::string key;
    WSType type;

    wsp_arg_t()
    {

    }

    wsp_arg_t(const char* n, const char* k, WSType t):
      name(n), key(k), type(t)
    {

    }

    json toJSON() {
      json out = json::object();
      out["n"] = name;
      out["k"] = key;
      out["t"] = type;

      return out;
    }
  };

  struct wsp_endpoint_t {
    std::string urn;
    std::string uri;
    std::vector<wsp_arg_t> args;

    wsp_endpoint_t()
    {

    }

    wsp_endpoint_t(const char* _urn, const char* _uri, std::vector<wsp_arg_t> _args):
      urn(_urn), uri(_uri), args(_args)
    {

    }

    json toJSON() {
      json out = json::object();
      out["urn"] = urn;
      out["uri"] = uri;

      out["args"] = json::array();
      for(auto it = args.begin(); it != args.end(); it++)
        out["args"].push_back(it->toJSON());

      return out;
    }
  };

  struct wsp_t {
    std::string urn;
    std::string uri;
    std::string ver;
    std::vector<wsp_endpoint_t> rscs;

    WSProtocol* handler;

    wsp_t()
    {

    }

    wsp_t(const char* _urn, const char* _uri, const char* _ver, std::vector<wsp_endpoint_t> _rscs, WSProtocol* _handler):
      urn(_urn), uri(_uri), ver(_ver), rscs(_rscs), handler(_handler)
    {

    }

    json toJSON() {
      json out;

      if(urn != "") {
        out["urn"] = urn;
        out["uri"] = uri;
        out["ver"] = ver;

        out["rscs"] = json::array();
        for(auto it = rscs.begin(); it != rscs.end(); it++)
          out["rscs"].push_back(it->toJSON());

      } else {
        out = json::array();
        for(auto it = rscs.begin(); it != rscs.end(); it++)
          out.push_back(it->toJSON());
      }

      return out;
    }
  };



  class WSProtocol {
  public:
    typedef unsigned char* data_t;
    typedef uint16_t msg_id_t;

    struct route_t {
      size_t id = 0;
      std::string uri = "";
      WSProtocol* wsp = nullptr;
    };

    typedef std::map<std::string, route_t> route_map_t;

    struct Message {
      json dat;
      lws* res; // Response write handle

      std::string getURI() {
        return dat[WSProtocol::JS_URI];
      }

      //std::string getRaw()
    };


    // Binary interface structures
    struct Packet {
      void* ptr;
      size_t len;
    };

    struct __attribute__((packed)) Request {
      msg_id_t id;
      uint8_t cmd;

      void* getData() { return (&(this->cmd)) + 1; }
    };

    struct __attribute__((packed)) Response {
      msg_id_t id;
    };

    struct __attribute__((packed)) Event {
      char* name, *data;

      std::string getData() {
        std::stringstream out;
        out << name << '\n'<< data << '\n';
        return out.str();
      }
    };
    // ****

    WSProtocol();
    WSProtocol(const wsp_t);

    int recv(Packet);

    int validate(Packet*);

    //int route(Packet*);
    virtual int route(const Message&);

    virtual wsp_t* getDescriptor();

    json getProtocols();

    void setWS(WebSocketServer* ptr);

    size_t add(const wsp_t*);

    void remove(size_t);

    int send(json&);

    template<typename T>
    static int send(lws *wsi, Request*& req, T dat) {
      Response res;
      res.id = req->id;

      int len = sizeof(res) + sizeof(dat);

      data_t buf = (data_t)malloc(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING);

      data_t dst = &buf[LWS_SEND_BUFFER_PRE_PADDING];
      memcpy(dst, (data_t)&res, sizeof(res));
      dst += sizeof(res);
      memcpy(dst, (data_t)&dat, sizeof(dat));

      lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_BINARY);
      free(buf);

      return 0;
    }

    template<typename T>
    static int send(lws *wsi, T dat, size_t len) {
      data_t buf = (data_t)malloc(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING);

      data_t dst = &buf[LWS_SEND_BUFFER_PRE_PADDING];
      memcpy(dst, (data_t)&dat, len);

      lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_TEXT);
      free(buf);

      return 0;
    }

    static int emit(lws *wsi, Event ev) {
      std::string data = ev.getData();
      size_t len = data.size();

      data_t buf = (data_t)malloc(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING);

      data_t dst = &buf[LWS_SEND_BUFFER_PRE_PADDING];
      memcpy(dst, data.c_str(), len);

      lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_BINARY);
      free(buf);

      return 0;
    }

  public:
    static const char *JS_M_ID, *JS_NSP, *JS_URI;

  protected:
    WebSocketServer* mWS;
    wsp_t m_wsp;

  private:
    size_t route_id_seq;
    WSProtocol::route_map_t mRouteMap;

  };

}}

#endif