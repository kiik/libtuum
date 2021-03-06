/** @file tuum_http.cpp
 *  @brief Dirty implementation of http protocols.
 *
 * @authors Meelik Kiik
 * @date 18. October 2016
 * @version 0.1
 */

#include <libwebsockets.h>

#include "platform.hpp"
#include "hal.hpp"
#include "lpx_iformat.hpp"

#include "tuum_lpx.hpp"
#include "tuum_visioning.hpp"

#include "tuum_http.hpp"


namespace tuum { namespace http {

  const char* mjpeg_head = "HTTP/1.1 200 OK\nCache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\nConnection: close\nContent-Type: multipart/x-mixed-replace;boundary=--FRAME--\nExpires: Mon, 3 Jan 2000 12:34:56 GMT\nPragma: no-cache\n\n";
  const char* mjpeg_boundary = "\n--FRAME--\n";


  long long stat_time = 0;
  size_t stat_seq = 0;

  void stat_begin() {
    if(stat_seq > 2) return;
    stat_time = micros();
  }

  void stat_end() {
    if(stat_seq > 2) return;
    printf("stat(%lu): %llu us\n", stat_seq++, (micros() - stat_time));
  }


  size_t lframe = 0;
  size_t t0 = 0;
  float dt = 0;

  ImageStream* camStream;
  bool done = false;

  void fps() {
    size_t t1 = millis();
    dt = dt * 0.7 + (t1-t0) * 0.3;
    printf("FPS=%f.0\n", 1000.0 / dt);
    t0 = t1;
  }

  size_t read_data_stream(size_t lid, image_t& out) {
    tuum::Visioning* vis = tuum::gVision;
    if(vis == nullptr) return 0;
    if(vis->readFrame(out) < 0) return 0;
    return out->id;
  }

  void mjpeg_headers(lws *wsi) {
    lws_write(wsi, (unsigned char*)mjpeg_head, strlen(mjpeg_head), LWS_WRITE_HTTP);
  }

  void mjpeg_frame(lws *wsi, size_t len) {
    std::stringstream head;
    head << "X-Timestamp: " << millis() << std::endl;
    head << "Content-Length: " << len << std::endl;
    head << "Content-Type: " << "image/png" << std::endl;
    head << std::endl; // End of header
    std::string _head = head.str();

    lws_write(wsi, (unsigned char*)(_head.c_str()), _head.size(), LWS_WRITE_HTTP);
  }

  image_t img;

  void mjpeg_stream(lws *wsi) {
    if(!img) {
      RTXLOG("Initializing stream buffer.", LOG_WARN);
      Env::spawnBuffer(img);
    }

    size_t fid = read_data_stream(lframe, img);

    if(img <= 0) return;
    lframe = fid;
    //fps();

    image_t out_jpg = lpx::rgb_to_png(img);

    lws_write(wsi, (unsigned char*)mjpeg_boundary, strlen(mjpeg_boundary), LWS_WRITE_HTTP);

    size_t len = out_jpg->size;

    mjpeg_frame(wsi, len);
    lws_write(wsi, (unsigned char*)out_jpg->data, len, LWS_WRITE_HTTP);
  }

}}
