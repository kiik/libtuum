
#ifndef TUUM_CMV_H
#define TUUM_CMV_H

#include "cmv/cmv_data.hpp"
#include "cmv/cmv_Filter.hpp"

namespace tuum { namespace CMV {

  int rle(uint8_t*, size_t, const Filter&, BlobSet&);

  void region_segment(uint8_t*, size_t, const Filter&, RunlineSet&);
  void region_merge(RunlineSet, BlobSet&);

}}

#endif
