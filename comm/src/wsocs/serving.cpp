
#include <boost/thread.hpp>

#include "platform.hpp"

#include "wsocs/WebSocketServer.hpp"
#include "wsocs/serving.hpp"

namespace tuum {
namespace wsocs {

  const int WSOCS_SRV_N = 1;

  boost::thread* wsSrvThread = nullptr;

  WebSocketServer* wsock_srvs[WSOCS_SRV_N];

  soft_clk_t clk_100Hz(10);

  bool running = true;

  //FIXME: Generate callbacks per-server instance.
  int cb_http_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len) {
    if(wsock_srvs[0] != nullptr) wsock_srvs[0]->cb_http(wsi, reason, user, in, len);
  }

  int cb_wsjs_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len) {
    if(wsock_srvs[0] != nullptr) wsock_srvs[0]->cb_wsjs(wsi, reason, user, in, len);
  }

  int cb_wsbin_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len) {
    if(wsock_srvs[0] != nullptr) wsock_srvs[0]->cb_wsbin(wsi, reason, user, in, len);
  }

  int register_server(WebSocketServer* srv) {
    for(int i=0; i < WSOCS_SRV_N; i++) {
      if(wsock_srvs[i] != nullptr) continue;
      wsock_srvs[i] = srv;
      return 1;
    }

    return -1;
  }

  WebSocketServer* fetch_server() {
    return wsock_srvs[0];
  }

  int service_init() {
    for(int i=0; i < WSOCS_SRV_N; i++) wsock_srvs[i] = nullptr;
    return 0;
  }

  int service_bootstrap() {
    if(wsSrvThread == nullptr) {
      wsSrvThread = new boost::thread(&service_process);
      wsSrvThread->detach();
    }
    return 0;
  }

  void service_process() {
    while(running) {
      if(clk_100Hz.tick()) {
        for(int i=0; i < WSOCS_SRV_N; i++) {
          if(wsock_srvs[i] != nullptr) wsock_srvs[i]->process();
        }
      }

      clk_100Hz.wait();
    }
  }

}}
