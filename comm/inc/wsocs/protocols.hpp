
#ifndef WSOCS_PROTOCOL_H
#define WSOCS_PROTOCOL_H

#include <libwebsockets.h>

namespace tuum {
namespace wsocs {

  int cb_http_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len);
  int cb_wsjs_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len);
  int cb_wsbin_proxy(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len);

  extern lws_protocols gProtocols[];

  const lws_extension gExts[] = {
    { NULL, NULL, NULL /* terminator */ }
  };

}}

#endif
