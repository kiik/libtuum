
#include "sys/clab_pass.hpp"

namespace tuum {
namespace ocl {

  Pass::Pass()
  {

  }

  void Pass::add(const procedure_t in) {
    mProcedures.push_back(in);
  }

  void Pass::clear() {
    mProcedures.clear();
  }

  int Pass::bind(Pipeline* ppl, cl::CommandQueue* _q)
  {
    size_t seq;
    cl::Buffer* buf_ptr;
    cl::Kernel* kn;

    for(auto it = mProcedures.begin(); it != mProcedures.end(); it++) {
      kn = it->kernel;
      seq = 0;

      for(auto buf_it = it->args.begin(); buf_it != it->args.end(); buf_it++) {
        /*
        if(ppl->getBuffer(*buf_it, buf_ptr) < 0) {
          RTXLOG(format("Error - Buffer '%s' not found!", *buf_it));
          return -1;
        }*/

        kn->setArg(seq++, *buf_ptr);
      }

      _q->enqueueNDRangeKernel(*kn, it->gOffset, it->gSize, it->lSize);
    }

    return 1;
  }

}}
