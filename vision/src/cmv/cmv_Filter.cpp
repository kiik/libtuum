
#include "cmv/cmv_Filter.hpp"

namespace tuum { namespace CMV {

  FilterBase::FilterBase():
    id_seq(1)
  {

  }

  int FilterBase::createId(uint32_t& out) {
    if(id_seq >= (1 << 31)) return -1;
    out = id_seq;
    id_seq = id_seq << 1;
    return 0;
  }

  YUVFilter::YUVFilter():
    FilterBase()
  {
    size_t i;
    for(i = 0; i < 16; i++) clss_Y[i] = 0;
    for(i = 0; i < 32; i++) {
      clss_U[i] = 0;
      clss_V[i] = 0;
    }
  }

  int YUVFilter::addClass(ColorClass cls) {
    if(cls.id == 0)
      if(createId(cls.id) < 0) return -1;

    // Convect colorspan to lower resolution, iterate & insert spans into classifiers
    for(size_t i = cls.mn[0] * U8_16CLS_REDUCTION; i <= cls.mx[0] * U8_16CLS_REDUCTION; i++) clss_Y[i] |= cls.id;
    for(size_t i = cls.mn[1] * U8_32CLS_REDUCTION; i <= cls.mx[1] * U8_32CLS_REDUCTION; i++) clss_U[i] |= cls.id;
    for(size_t i = cls.mn[2] * U8_32CLS_REDUCTION; i <= cls.mx[2] * U8_32CLS_REDUCTION; i++) clss_V[i] |= cls.id;

    printf(":cmv::Filter: New class id=%lu\n", cls.id);
    return 0;
  }

  uint32_t YUVFilter::operator()(uint8_t y, uint8_t u, uint8_t v)
  {
    return clss_Y[y] && clss_U[u] && clss_V[v];
  }

  UVFilter::UVFilter():
    FilterBase()
  {
    size_t i, j;
    for(i = 0; i < 16; i++) {
      for(j = 0; j < 32; j++) {
        clss_U[i][j] = 0;
        clss_V[i][j] = 0;
      }
    }
  }

  int UVFilter::addClass(ColorClass cls) {
    if(cls.id == 0)
      if(createId(cls.id) < 0) return -1;

    // Convect colorspan to lower resolution, iterate & insert spans into classifiers
    uint16_t y;
    for(auto it = cls.clss.begin(); it != cls.clss.end(); it++) {
      y = it->Y * U8_16CLS_REDUCTION;
      for(size_t i = it->U[0] * U8_32CLS_REDUCTION; i <= it->U[1] * U8_32CLS_REDUCTION; i++) clss_U[y][i] |= cls.id;
      for(size_t i = it->V[0] * U8_32CLS_REDUCTION; i <= it->V[1] * U8_32CLS_REDUCTION; i++) clss_V[y][i] |= cls.id;
    }

    printf(":cmv::UVFilter: New class id=%u\n", cls.id);
    return 0;
  }

  uint32_t UVFilter::operator()(uint8_t y, uint8_t u, uint8_t v)
  {
    return clss_U[y][u] && clss_V[y][v];
  }

}}
