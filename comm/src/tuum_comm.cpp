
#include "tuum_comm.hpp"

#include "wsocs/serving.hpp"

namespace tuum { namespace comm {

  int register_server(wsocs::WebSocketServer* ptr) {
    return wsocs::register_server(ptr);
  }

  int init() {
    wsocs::service_init();
    return 0;
  }

  void setup() {
    wsocs::service_bootstrap();
  }

  void process() {

  }

}}
