
#include "tuum_threshold.hpp"

namespace tuum { namespace vision {

  size_t BALL_MASK = 0b1;

  struct CMVClass {
    uint8_t mn[3], mx[3];
  };

  CMVClass BALL = {51, 106, 148, 67, 114, 162};

  CMVClass RGB_BALL = {130, 44, 30, 198, 112, 98};

  struct CMVFilter {
    uint32_t clss_Y[16], clss_U[32], clss_V[32];

    size_t id_seq;

    CMVFilter():
      id_seq(1)
    {
      size_t i;
      for(i = 0; i < 16; i++) clss_Y[i] = 0;
      for(i = 0; i < 32; i++) {
        clss_U[i] = 0;
        clss_V[i] = 0;
      }

      uint32_t id;
      addClass(BALL, id);
    }

    int addClass(CMVClass cls, uint32_t& out) {
      if(getId(out) < 0) return -1;
      for(size_t i = cls.mn[0] * U8_16CLS_REDUCTION; i <= cls.mx[0] * U8_16CLS_REDUCTION; i++) clss_Y[i] |= out;
      for(size_t i = cls.mn[1] * U8_32CLS_REDUCTION; i <= cls.mx[1] * U8_32CLS_REDUCTION; i++) clss_U[i] |= out;
      for(size_t i = cls.mn[2] * U8_32CLS_REDUCTION; i <= cls.mx[2] * U8_32CLS_REDUCTION; i++) clss_V[i] |= out;
      return 0;
    }

    int getId(uint32_t& out) {
      if(id_seq >= (1 << 31)) return -1;
      out = id_seq;
      id_seq = id_seq << 1;
      return 0;
    }

  };

  CMVFilter gFlt;


  void classify_colors(uint8_t* dat, size_t length) {
    size_t i, clss;
    uint8_t y, u, v;

    for(i = 0; i < length; i += 3) {
      y = dat[i] * U8_16CLS_REDUCTION;
      u = dat[i + 1] * U8_32CLS_REDUCTION;
      v = dat[i + 2] * U8_32CLS_REDUCTION;

      clss = gFlt.clss_Y[y] && gFlt.clss_U[u] && gFlt.clss_V[v];

      // Threshold
      if(clss == 0) {
        dat[i] = 0;
        dat[i + 1] = 0;
        dat[i + 2] = 0;
      }
    }

  }

}}
