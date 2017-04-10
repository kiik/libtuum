
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

  const lws_protocols gProtocols[] = {
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

  const lws_extension gExts[] = {
    { NULL, NULL, NULL /* terminator */ }
  };

}}

#endif
