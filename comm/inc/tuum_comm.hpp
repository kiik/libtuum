/** @file tuum_comm.hpp
 *  Communication module interface.
 *
 *  @authors Meelik Kiik
 *  @version 0.2
 *  @date 10. September 2016
 */

#ifndef TUUM_COMM_H
#define TUUM_COMM_H

#include "wsocs/serving.hpp"

namespace tuum { namespace comm {

  int register_server(wsocs::WebSocketServer*);

  int init();

  void setup();
  void process();

}}

#endif
