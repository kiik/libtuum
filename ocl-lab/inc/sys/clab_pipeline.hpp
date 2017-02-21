
#ifndef CLAB_PIPELINE_H
#define CLAB_PIPELINE_H

namespace tuum {
namespace ocl {
namespace lab {

  class Pipeline {
  public:
    Pipeline(); // BufferSet, PassSet

    int init();
    int setup();
    int process();

  protected:
    // BufferSet mBuffers;
    // PassSet mPasses;
  };

}}}

#endif
