
#include "cmv/cmv_Filter.hpp"

namespace tuum { namespace CMV {

  Filter::Filter():
    id_seq(1)
  {
    size_t i;
    for(i = 0; i < 16; i++) clss_Y[i] = 0;
    for(i = 0; i < 32; i++) {
      clss_U[i] = 0;
      clss_V[i] = 0;
    }

    /*
    uint32_t id;
    addClass(BALL, id);*/
  }

  int Filter::addClass(ColorClass cls, uint32_t& out) {
    if(createId(out) < 0) return -1;
    for(size_t i = cls.mn[0] * U8_16CLS_REDUCTION; i <= cls.mx[0] * U8_16CLS_REDUCTION; i++) clss_Y[i] |= out;
    for(size_t i = cls.mn[1] * U8_32CLS_REDUCTION; i <= cls.mx[1] * U8_32CLS_REDUCTION; i++) clss_U[i] |= out;
    for(size_t i = cls.mn[2] * U8_32CLS_REDUCTION; i <= cls.mx[2] * U8_32CLS_REDUCTION; i++) clss_V[i] |= out;
    return 0;
  }

  int Filter::rmvClass(ColorClass cls) {
    return -1; //TODO
  }

  int Filter::createId(uint32_t& out) {
    if(id_seq >= (1 << 31)) return -1;
    out = id_seq;
    id_seq = id_seq << 1;
    return 0;
  }

}}
