
#ifndef WSOCS_SERVING_H
#define WSOCS_SERVING_H

#include "wsocs/declarations.hpp"

namespace tuum {
namespace wsocs {

  WebSocketServer* fetch_server();

  int register_server(WebSocketServer*);

  int service_init();
  int service_bootstrap();

  void service_process();

}}

#endif
