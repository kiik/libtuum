
#include "tuum_wsocs.hpp"

namespace tuum { namespace wsocks {

  WebSocketServer::WebSocketServer():
    m_port(8080), m_opts(0),
    m_iface(nullptr), m_ctx(nullptr),
    m_cert_path(nullptr), m_key_path(nullptr),
    m_protocols(nullptr), m_exts(nullptr)
  {

  }

  WebSocketServer::~WebSocketServer() {
    lws_context_destroy(m_ctx);
  }

  int WebSocketServer::init() {
    if(loadProtocols() < 0) return -1;

    memset(&m_info, 0, sizeof(m_info));
    m_info.port = m_port;
    m_info.iface = m_iface;
    m_info.protocols = gProtocols;
    m_info.extensions = gExts;
    m_info.ssl_cert_filepath = NULL;
    m_info.ssl_private_key_filepath = NULL;
    m_info.gid = -1;
    m_info.uid = -1;
    m_info.options = m_opts;

    RTXLOG("Creating wsoc context...", LOG_INFO);
    m_ctx = lws_create_context(&m_info);

    if(m_ctx == nullptr){
      RTXLOG("libwebsocket init failed!", LOG_ERR);
      return -2;
    }

    return 0;
  }

  int WebSocketServer::loadProtocols() {
    return 0;
  }

  void WebSocketServer::process() {
    bool running = true;
    while(running) {
      lws_service(m_ctx, 0);
    }
  }

  int WebSocketServer::cb_http(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len)
  {
    switch(reason) {
      case LWS_CALLBACK_HTTP:
        return 0;
      case LWS_CALLBACK_HTTP_FILE_COMPLETION:
        return 0;
      case LWS_CALLBACK_HTTP_WRITEABLE:
        return 0;
      case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
        return 0;
      default:
        break;
    }

    return 0;
  }


  int WebSocketServer::cb_wsoc(lws *wsi, lws_callback_reasons reason,
                  void *user, void *in, size_t len)
  {
    uint8_t *raw = (uint8_t*)in;
    switch(reason) {
      case LWS_CALLBACK_ESTABLISHED:
        printf("[WSS:cb_wsoc]Connection established.\n");
        onConnect();
        lws_callback_on_writable(wsi);
        break;
      case LWS_CALLBACK_RECEIVE:
        onMessage(wsi, in, len);
        break;
      case LWS_CALLBACK_SERVER_WRITEABLE:
        lws_callback_on_writable(wsi);
        break;
    }

    return 0;
  }

}}