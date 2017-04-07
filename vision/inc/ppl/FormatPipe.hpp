
#ifndef TUUM_COLORPIPE_H
#define TUUM_COLORPIPE_H

#include "ppl/PipeBase.hpp"

namespace tuum { namespace vision {

  class FormatPipe : public PipeBase
  {
  public:
    FormatPipe();

    int init();

    int pass(image_t&);
  };

}}

#endif
