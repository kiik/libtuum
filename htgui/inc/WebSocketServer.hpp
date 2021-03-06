
#ifndef TUUM_WSOCS_H
#define TUUM_WSOCS_H

#include <libwebsockets.h>

#include "json.hpp"
#include "tuum_platform.hpp"
#include "tuum_wsproto.hpp"

using json = nlohmann::json;

namespace tuum { namespace wsocs {

  class WebSocketServer
  {
  public:
    WebSocketServer();
    ~WebSocketServer();

    int init();
    void process();

    int cb_http(lws *wsi, lws_callback_reasons reason,
                    void *user, void *in, size_t len);

    int cb_wsjs(lws *wsi, lws_callback_reasons reason,
                    void *user, void *in, size_t len);

    int cb_wsbin(lws *wsi, lws_callback_reasons reason,
                    void *user, void *in, size_t len);

    WSProtocol* proto();

    struct ctx_t {
      lws* wsi = nullptr;
      size_t mId = 0;
    };

  protected:
    ctx_t* mCtx;

  private:
    int m_port, m_opts;

    const char *m_iface,
               *m_cert_path,
               *m_key_path;

    struct lws_context *m_ctx;
    struct lws_context_creation_info m_info;

    struct lws_protocols *m_protocols;
    struct lws_extension *m_exts;

    int loadProtocols();

  public:
    virtual void onGet() {};

    virtual void onConnect() {};
    virtual void onMessage(lws*, void*, size_t) {};
    virtual void onMessage(WSProtocol::Message) {};

    virtual int send(json&);

  private:
      WSProtocol mProtocol;
  };

}}

#endif // TUUM_WSOCS_H
