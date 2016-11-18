
#ifndef TUUM_CMV_FILTER_H
#define TUUM_CMV_FILTER_H

#include "cmv/cmv_data.hpp"

namespace tuum { namespace CMV {

  class Filter
  {
  public:

    Filter();

    int addClass(ColorClass, uint32_t&);
    int rmvClass(ColorClass);

    int createId(uint32_t&);

  public:
    uint32_t clss_Y[16], clss_U[32], clss_V[32];

    size_t id_seq;
  };

}}

#endif
