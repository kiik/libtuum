
#include "wsocs/WebSocketServer.hpp"

#include "wsocs/serving.hpp"

#include "wsocs/protocols.hpp"

namespace tuum {
namespace wsocs {

  //FIXME: Generate callbacks per-server instance.
  int cb_http_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len) {
    WebSocketServer* ptr = fetch_server();
    if(ptr == nullptr) return -1;
    ptr->cb_http(wsi, reason, user, in, len);
    return 1;
  }

  int cb_wsjs_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len) {
    WebSocketServer* ptr = fetch_server();
    if(ptr == nullptr) return -1;
    ptr->cb_wsjs(wsi, reason, user, in, len);
    return 1;
  }

  int cb_wsbin_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len) {
    WebSocketServer* ptr = fetch_server();
    if(ptr == nullptr) return -1;
    ptr->cb_wsbin(wsi, reason, user, in, len);
    return 1;
  }

  lws_protocols gProtocols[] = {
    {
      "http-only",
      cb_http_proxy,
      sizeof(size_t),
    },
    {
      "ws-json",
      cb_wsjs_proxy,
      0
    },
    {
      "ws-binary",
      cb_wsbin_proxy,
      0
    },
    {
      NULL, NULL, 0
    }
  };

}}
