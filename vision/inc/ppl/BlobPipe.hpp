
#ifndef TUUM_BLOBPIPE_H
#define TUUM_BLOBPIPE_H

#include "ppl/PipeBase.hpp"

namespace tuum { namespace vision {

  class BlobPipe : public PipeBase
  {
  public:
    BlobPipe();

    int init();

    int pass(image_t&);
  };

}}

#endif
