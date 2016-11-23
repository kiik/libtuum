
#ifndef TUUM_DRAW_H
#define TUUM_DRAW_H

#include "tuum_buff.hpp"
#include "tuum_cmv.hpp"

namespace tuum {

  static size_t to_img_offset(size_t x, size_t y, size_t stride) {
    return ((y * stride) + x) * 3;
  }

  static void draw_point(size_t x, size_t y, uint8_t* dat, size_t stride) {
    size_t os = to_img_offset(x, y, stride);
    dat[os] = 255;
    dat[os + 1] = 0;
    dat[os + 2] = 0;
  }

  enum Colors {
    RED,
    GREEN,
    BLUE,
  };

  static void draw_line(image_t out, CMV::rect_t r, Colors c = Colors::RED) {
    uint8_t* dat = (uint8_t*)out->data;
    size_t os, W = out->frm.width;

    double y;
    double dy = (r.y1 - r.y0), dx = (r.x1 - r.x0);

    size_t len = (size_t)sqrt(dy*dy + dx*dx);

    for(double z = 0; z < len; z++) {
      double p = z / len;
      os = to_img_offset(r.x0 + (size_t)(p * dx), r.y0 + (size_t)(p * dy), W);

      switch(c) {
        case RED:
          dat[os] = 255;
          dat[os + 1] = 0;
          dat[os + 2] = 0;
          break;
        case GREEN:
          dat[os] = 0;
          dat[os + 1] = 255;
          dat[os + 2] = 0;
          break;
      }

    }
  }

  static void draw_rect(image_t out, CMV::rect_t r) {
    uint8_t* dat = (uint8_t*)out->data;

    Colors c = Colors::RED;

    if(r.getArea() > 100) {
      c = Colors::GREEN;
    }

    draw_line(out, {r.x0, r.y0, r.x1, r.y0}, c);
    draw_line(out, {r.x0, r.y1, r.x1, r.y1}, c);

    draw_line(out, {r.x0, r.y0, r.x0, r.y1}, c);
    draw_line(out, {r.x1, r.y0, r.x1, r.y1}, c);
  }
}

#endif
