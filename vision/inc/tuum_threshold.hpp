
#ifndef TUUM_THRESHOLD_H
#define TUUM_THRESHOLD_H

#include "platform.hpp"

namespace tuum { namespace vision {

  const float U8_16CLS_REDUCTION = 15.0 / 255.0;
  const float U8_32CLS_REDUCTION = 31.0 / 255.0;

  void classify_colors(uint8_t*, size_t);

}}

#endif
