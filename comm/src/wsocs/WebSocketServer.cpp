
#include <libwebsockets.h>

#include "tuum_wsproto.hpp"
#include "tuum_http.hpp"
#include "wsocs/protocols.hpp"

#include "WSContext.hpp"

#include "wsocs/WebSocketServer.hpp"

namespace tuum {
namespace wsocs {

  WSContext wsCtx;

  typedef uint8_t* data_t;

  WebSocketServer::WebSocketServer():
    mCtx(nullptr),
    m_port(8079), m_opts(0),
    m_iface(nullptr), m_ctx(nullptr),
    m_cert_path(nullptr), m_key_path(nullptr),
    m_protocols(nullptr), m_exts(nullptr)
  {
    int v = 0;
    char** argv = nullptr;

    mProtocol.setWS(this);
  }

  WebSocketServer::~WebSocketServer() {
    lws_context_destroy(m_ctx);
  }

  int WebSocketServer::init() {
    if(loadProtocols() < 0) return -1;

    /*
    RTXLOG("Set lws log...", LOG_INFO);
    lws_set_log_level(0b11111111111, NULL);
    */

    RTXLOG("Creating context...", LOG_INFO);
    m_ctx = lws_create_context(&m_info);

    if(m_ctx == nullptr) {
      RTXLOG("libwebsocket context creation failed!", LOG_ERR);
      return -2;
    }

    return 1;
  }

  int WebSocketServer::loadProtocols() {
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

    return 0;
  }

  size_t WebSocketServer::registerProtocol(WSProtocol* wsp)
  {
    wsp->setWS(this);
    return proto()->add(wsp->getDescriptor());
  }

  void WebSocketServer::process() {
    lws_service(m_ctx, 0);
  }

  int WebSocketServer::cb_http(lws *wsi, lws_callback_reasons reason, void *user, void *in, size_t len)
  {
    WSContext::id_t* cId = (size_t*)user;

    switch(reason) {
      case LWS_CALLBACK_FILTER_HTTP_CONNECTION:
        {
          WSContext::ctx_t ctx = wsCtx.createContext();
          printf("[WSS:cb_http]New connection id=%lu.\n", ctx->id);
          *cId = ctx->id;
          wsCtx.insert(ctx);
        }

        return 0;
      case LWS_CALLBACK_HTTP:

        //http::mjpeg_headers(wsi);
        lws_callback_on_writable(wsi);

        return 0;
      case LWS_CALLBACK_HTTP_FILE_COMPLETION:
        return 0;
      case LWS_CALLBACK_HTTP_WRITEABLE:
      {
        WSContext::ctx_t ctx;
        if(wsCtx.find(*cId, ctx) >= 0) {
          //http::mjpeg_stream(wsi);
          lws_callback_on_writable(wsi);
        } else {
          printf("[WSS:cb_http]Unknown id %lu.\n", *cId);
        }
        return 0;
      }
      case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
        return 0;
      default:
        break;
    }

    return 0;
  }

  int WebSocketServer::cb_wsjs(lws *wsi, lws_callback_reasons reason, void *user, void *in, size_t len)
  {
    uint8_t *raw = (uint8_t*)in;

    mCtx = new ctx_t();
    mCtx->wsi = wsi;
    mCtx->mId = 0;

    switch(reason) {
      case LWS_CALLBACK_ESTABLISHED:
        printf("[WSS:cb_wsjs]Connection established.\n");
        onConnect();
        //lws_callback_on_writable(wsi);
        break;
      case LWS_CALLBACK_RECEIVE:
        {
          if(len <= 0) break;
          bool err_flag = false;
          json data;

          //TODO: Error handling
          try {
            data = json::parse((char*)in);
          } catch (const std::invalid_argument& err) {
            err_flag = true;
            printf("[WSS::cb_wsjs]%s ( '%s' )\n", err.what(), (char*)in);
            //TODO: 'onError' callback
          }

	  size_t mId = 0;
          if(data.find(WSProtocol::JS_M_ID) != data.end()) {
            mId = data[WSProtocol::JS_M_ID].get<size_t>(); 
	  } else err_flag = true;

          if(!err_flag) {
            if(mId != 0) {
              onMessage({data, wsi});
	    } else pong();

            try {
            }
            catch (const std::domain_error& err) {
              printf("[WSS::cb_wsjs]%s ( '%s' )\n", err.what(), (char*)in);
              err_flag = true;
              //TODO: 'onError' callback
            }
          }
         }
        break;
      case LWS_CALLBACK_SERVER_WRITEABLE:
        {
          //WSProtocol::Event ev;

          /*
          ev.name = new char[8];
          ev.data = new char[7];
          strcpy(ev.name, "message");
          strcpy(ev.data, "123456");
          printf("DBG: %s, %s\n", ev.name, ev.data);
          */

        }
        lws_callback_on_writable(wsi);
        break;
    }

    delete(mCtx);
    mCtx = nullptr;

    return 0;
  }

  int WebSocketServer::cb_wsbin(lws *wsi, lws_callback_reasons reason, void *user, void *in, size_t len)
  {
    uint8_t *raw = (uint8_t*)in;
    switch(reason) {
      case LWS_CALLBACK_ESTABLISHED:
        printf("[WSS:cb_wsbin]Connection established.\n");
        onConnect();
        //lws_callback_on_writable(wsi);
        break;
      case LWS_CALLBACK_RECEIVE:
        onMessage(wsi, in, len);
        break;
      case LWS_CALLBACK_SERVER_WRITEABLE:
        {
          //WSProtocol::Event ev;

          /*
          ev.name = new char[8];
          ev.data = new char[7];
          strcpy(ev.name, "message");
          strcpy(ev.data, "123456");
          printf("DBG: %s, %s\n", ev.name, ev.data);
          */

        }
        lws_callback_on_writable(wsi);
        break;
    }

    return 0;
  }

  WSProtocol* WebSocketServer::proto() { return &mProtocol; }

  void WebSocketServer::pong() {
    json dat = json::object();
    dat[WSProtocol::JS_M_ID] = 0;
    send(dat);
  }

  int WebSocketServer::send(json& dat) {
    if(mCtx == nullptr) return -1;
    //FIXME: Queue up passed messages
    if(lws_send_pipe_choked(mCtx->wsi)) {
      //printf("[WS::send<CHOKE>]Passing '%s'\n", dat.dump().c_str());    
      return -2;
    }

    WSProtocol::Response res;

    std::string s = dat.dump();

    size_t len = s.size();
    data_t buf = (data_t)malloc(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING);

    data_t dst = &buf[LWS_SEND_BUFFER_PRE_PADDING];
    memcpy(dst, s.c_str(), len);

    // printf("1 %i (%i)\n", len, lws_send_pipe_choked(mCtx->wsi));
    lws_write(mCtx->wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_TEXT);
    // printf("2 %i (%i)\n", len, lws_send_pipe_choked(mCtx->wsi));

    free(buf);

    return 0;
  }

}}
