/** @file tuum_comm.cpp
 *  Communication module interface.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 10. September 2016
 */

#include "wsocs/WebSocketServer.hpp"
#include "wsocs/serving.hpp"

#include "tuum_comm.hpp"

namespace tuum { namespace comm {

  int init()
  {
    wsocs::service_init();
    return 0;
  }

  void setup()
  {
    wsocs::service_bootstrap();
    wsocs::register_server(new wsocs::WebSocketServer());
  }

  void process()
  {

  }

}}
